#include "../include/argparse.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <map>

using std::map;
using std::cerr;
using std::cout;
using std::endl;
using std::stoul;
using std::vector;
using std::string;
using std::wstring;

namespace fs = std::filesystem;

/**
 * @enum LevelState
 * @brief Represents the iteration state of a directory level.
 */
enum LevelState {
    NOT_ITERATING, ///< Indicates the last entry at the current level.
    ITERATING,     ///< Indicates there are more entries to process at the current level.
    NO_VALUE       ///< Indicates no specific state (used for the root level).
};

// Maps each directory depth level to its current iteration state.
map<int, LevelState> level_states;

// Other variables
unsigned int directory_count = 0;
unsigned int file_count = 0;

// Function Declarations
unsigned int get_directory_entry_count(const string& directory_path);
void process_directory_entries(
    const string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth,
    unsigned int entry_count,
    bool sort_entries = true,
    const vector<string>& ignore_list = {}
);
void generate_directory_hierarchy(
    string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth = 0,
    bool sort_entries = true,
    const vector<string>& ignore_list = {}
);
bool path_is_valid(
    const string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth
);
string generate_hierarchy_format_string(LevelState state);
string generate_character_string(unsigned int n, string s);
string generate_x_padding_string(unsigned int depth, unsigned int x_spacing);
string generate_entry_string(
    const string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth
);

/**
 * @brief Generates the hierarchy format string based on the level state.
 *
 * @param state The current state of the directory level.
 * @return A string representing the hierarchy symbol.
 */
string generate_hierarchy_format_string(LevelState state) {
	switch (state) {
		case ITERATING:
			return "├";
		case NOT_ITERATING:
			return "└";
		case NO_VALUE:
			return "";
	}
	return "";
}

/**
 * @brief Generates a repeated character string.
 *
 * @param n The number of times to repeat the string.
 * @param s The string to repeat.
 * @return The concatenated string.
 */
string generate_character_string(unsigned int n, string s) {
	string result;
	result.reserve(n * s.length());
	for (unsigned int i = 0; i < n; ++i)
		result += s;
	return result;
}

/**
 * @brief Generates the padding string on the x-axis based on depth and 
 * spacing.
 *
 * @param depth The current depth in the directory hierarchy.
 * @param x_spacing The number of spaces for horizontal padding.
 * @return The padding string.
 */
string generate_x_padding_string(unsigned int depth, unsigned int x_spacing) {
	// Generate padding string
	string padding_string = ""; 
	for (unsigned int level = 1; level < depth; level++) {
		// Retrieve state of each level
		auto it = level_states.find(level);
		if (it == level_states.end()) { // Level doesn't exist
			cerr << "Level " << level << " doesn't exist!" << endl;
			exit(1);
		}
		LevelState level_state = level_states[level];
		// Append vertical line or space based on level state
		padding_string += (level_state == ITERATING ? "│" : " ") 
			+ string(x_spacing, ' ');
	}
	return padding_string;
}

/**
 * @brief Constructs the string representation of a directory or file entry.
 *
 * @param path The name of the directory or file.
 * @param x_spacing The number of spaces for horizontal padding.
 * @param y_spacing The number of lines for vertical padding.
 * @param depth The current depth in the directory hierarchy.
 * @return The formatted entry string.
 */
string generate_entry_string(
	const string& path,
	unsigned int x_spacing,
	unsigned int y_spacing,
	unsigned int depth
) {
    if (level_states[depth] == NO_VALUE)
        return path;
    // Generate vertical padding
    string y_padding_string = "";
    for (unsigned int y = 0; y < y_spacing; y++) {
        if (depth > 0 || y > 0) // Avoid leading newline for the first entry
            y_padding_string += generate_x_padding_string(depth, x_spacing) + "│\n";
    }
    // Generate horizontal padding and hierarchy symbols
    string x_padding_string = generate_x_padding_string(depth, x_spacing);    
    x_padding_string += generate_hierarchy_format_string(level_states[depth]);
    x_padding_string += generate_character_string(x_spacing, "─");
    // Combine padding with the entry name
    return y_padding_string + x_padding_string + path;
}

/**
 * @brief Retrieves the total number of entries within a directory.
 *
 * @param directory_path The path to the directory.
 * @return The count of directory entries.
 */
unsigned int get_directory_entry_count(string& directory_path) {
    unsigned int count = 0;
    for (const auto& entry : fs::directory_iterator(directory_path))
        count++;
    return count; 
}

/**
 * @brief Validates the given path and handles it if it's a file or invalid.
 *
 * If the path is a file, it increments the file count, prints the file, and returns false
 * to indicate further processing should stop. If the path is invalid, it logs an error and
 * also returns false. If the path is a valid directory, it returns true to allow further processing.
 *
 * @param path The path to validate.
 * @param x_spacing The number of spaces for horizontal padding.
 * @param y_spacing The number of lines for vertical padding.
 * @param depth The current depth in the directory hierarchy.
 * @return true if the path is a valid directory, false otherwise.
 */
bool path_is_valid(
    const string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth
) {
    if (path.empty()) { 
        cerr << "Error: Path is empty!" << endl;
        return false;
    }
    // Check if the path is a file
    if (fs::is_regular_file(path)) {
        // Increment file count
        file_count++;
        // Print the file as a single entry
        string entry_string = generate_entry_string(
            path, x_spacing, y_spacing, depth
        );
        cout << entry_string << endl;
        return false; // Path is a file
    }
    // Check if the path is a directory
    if (!fs::is_directory(path)) {
        cerr << "Error: Path is neither a file nor a directory!" << endl;
        return false; // Invalid path
    }
    return true; // Path is a valid directory
}

/**
 * @brief Processes the entries in a directory and updates the hierarchy.
 *
 * @param path The current directory path.
 * @param x_spacing The number of spaces for horizontal padding.
 * @param y_spacing The number of lines for vertical padding.
 * @param depth The current depth in the directory hierarchy.
 * @param entry_count The total number of entries in the directory.
 * @param sort_entries Whether to sort directory entries before processing.
 * @param ignore_list List of file or directory names to ignore.
 */
void process_directory_entries(
    const string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth,
    unsigned int entry_count,
    bool sort_entries,
    const vector<string>& ignore_list
) {
    // Collect all directory entries
    vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(path)) {
        // Skip ignored names
        string name = entry.path().filename().string();
        auto it = std::find(ignore_list.begin(), ignore_list.end(), name);
        if (it != ignore_list.end()) continue;
        entries.push_back(entry);
    }
    // Sort entries if the flag is enabled
    if (sort_entries) {
        std::sort(entries.begin(), entries.end(), 
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                string filename_a = a.path().filename().string();
                string filename_b = b.path().filename().string();
                return filename_a < filename_b;
            }
        );
    }
    // Process entries
    int entry_index = 0;
    for (const auto& entry : entries) {
        entry_index++;
        // Update the level state based on entry position
        level_states[depth] = (entry_index != entry_count) 
            ? ITERATING
            : NOT_ITERATING;
        if (fs::is_regular_file(entry)) {
            // Increment file count
            file_count++;
            // Handle regular file
            string filename = entry.path().filename().string();
            string entry_string = generate_entry_string(
                filename, x_spacing, y_spacing, depth
            );
            cout << entry_string << endl;
        } else if (fs::is_directory(entry)) {
            // Increment directory count
            directory_count++;
            // Handle subdirectory recursively
            string entry_path = entry.path().string();
            generate_directory_hierarchy(
                entry_path, x_spacing, y_spacing, depth, sort_entries
            );
        }
    }
}

/**
 * @brief Recursively generates and prints the directory hierarchy.
 *
 * @param path The current directory path.
 * @param x_spacing The number of spaces for horizontal padding.
 * @param y_spacing The number of lines for vertical padding.
 * @param depth The current depth in the directory hierarchy.
 * @param sort_entries Whether to sort directory entries.
 * @param ignore_list List of file or directory names to ignore.
 */
void generate_directory_hierarchy(
    string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth,
    bool sort_entries,
    const vector<string>& ignore_list
) {
    // Validate the path
    if (!path_is_valid(path, x_spacing, y_spacing, depth)) return;
    // Ensure path ends with '/'
    if (path.back() != '/') 
        path += "/";
    // Determine the display name based on the level state
    string path_name = "";
    switch (level_states[depth]) {
        case NO_VALUE:
            path_name += path;
            break;
        default: // Extract the directory name from the path
            fs::path fs_path = path;
            path_name += fs_path.parent_path().filename().string();
            break;
    }
    // Ensure display name ends with '/'
    if (path_name.back() != '/')
        path_name += "/";
    // Create and print the entry string
    string entry_string = generate_entry_string(
        path_name, x_spacing, y_spacing, depth
    );
    cout << entry_string << endl;
    // Increment depth for recursion
    depth++;
    // Get the number of entries in the current directory
    unsigned int entry_count = get_directory_entry_count(path);
    // Process entries
    process_directory_entries(
        path, x_spacing, y_spacing, 
        depth, entry_count, sort_entries,
        ignore_list
    );
}

int main(int argc, char* argv[]) {
    // Initialize argparse
    argparse::ArgumentParser program("lstree", "1.0");
    // Define arguments
    program.add_argument("directory_path")
        .nargs(1)
        .default_value(vector<string>{"."})
        .help("Path to the directory to visualize. Defaults to the current directory.");
    program.add_argument("-x", "--x_spacing")
        .default_value(3)
        .scan<'i', int>() // Parse as integer
        .help("Horizontal spacing (number of spaces). Defaults to 3.");
    program.add_argument("-y", "--y_spacing")
        .default_value(1)
        .scan<'i', int>() // Parse as integer
        .help("Vertical spacing (number of lines). Defaults to 1.");
    program.add_argument("-s", "--sort")
        .default_value(true)
        .action([](const string& value) {
            if (value == "false" || value == "0") return false;
            if (value == "true" || value == "1") return true;
            throw std::runtime_error("Invalid value for --sort. Use 'true' or 'false'.");
        })
        .help("Enable or disable sorting of directory entries. Defaults to true.");
    program.add_argument("-i", "--ignore")
        .default_value(vector<string>{})
        .append()
        .help("List of file or directory names to ignore.");
    // Parse arguments
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        cerr << "Error: " << err.what() << endl;
        cout << program;
        return 1;
    }
    // Retrieve parsed values
    string directory_path = program.get<vector<string>>("directory_path")[0];
    int x_spacing = program.get<int>("--x_spacing");
    int y_spacing = program.get<int>("--y_spacing");
    bool sort_entries = program.get<bool>("--sort");
    vector<string> ignore_list = program.get<vector<string>>("--ignore");

    // TODO: Convert to class initializer
    // Initialize root level state
    level_states[0] = NO_VALUE;
    // Check if input path is a file
    if (fs::is_regular_file(directory_path)) {
        file_count = 1;
        generate_directory_hierarchy(directory_path, x_spacing, y_spacing, 0, sort_entries);
        cout << "\n0 directories, 1 file\n";
        return 0;
    }
    // If input is a directory, include root directory in the count
    if (fs::is_directory(directory_path)) {
        directory_count = 1; // Count the root directory
    }
    // Generate and print the directory hierarchy
    generate_directory_hierarchy(
        directory_path, 
        x_spacing, y_spacing, 
        0, sort_entries, ignore_list
    );
    // Print summary
    cout << "\n" << directory_count 
         << (directory_count == 1 ? " directory, " : " directories, ")
         << file_count 
         << (file_count == 1 ? " file\n" : " files\n");

    return 0;
}
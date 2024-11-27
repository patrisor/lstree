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
	string& path,
	unsigned int x_spacing,
	unsigned int y_spacing,
	unsigned int depth
) {
    if (level_states[depth] == NO_VALUE)
        return path;
    // Generate vertical padding
    string y_padding_string = "";
    for (unsigned int y = 0; y < y_spacing; y++)
        y_padding_string += generate_x_padding_string(depth, x_spacing) + "│\n";
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
 * @brief Recursively generates and prints the directory hierarchy.
 *
 * @param path The current directory path.
 * @param x_spacing The number of spaces for horizontal padding.
 * @param y_spacing The number of lines for vertical padding.
 * @param depth The current depth in the directory hierarchy.
 */
void generate_directory_hierarchy(
    string& path,
    unsigned int x_spacing,
    unsigned int y_spacing,
    unsigned int depth
) {
    // Validate path
    if (path.empty()) { 
        cerr << "Error: Path is empty!" << endl;
        return;
    }
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
    
    // TODO: Implement sorting of directory entries here
    
    int entry_index = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        entry_index++;
        // Update the level state based on entry position
        level_states[depth] = (entry_index != entry_count) 
            ? ITERATING
            : NOT_ITERATING;
        if (fs::is_regular_file(entry)) {
            // Handle regular file
            string filename = entry.path().filename().string();
            string entry_string = generate_entry_string(
                filename, x_spacing, y_spacing, depth
            );
            cout << entry_string << endl;
        }
        else if (fs::is_directory(entry)) {
            // Handle subdirectory recursively
            string entry_path = entry.path().string();
            generate_directory_hierarchy(
                entry_path, x_spacing, y_spacing, depth
            );
        }
    }

}

int main(int argc, char* argv[]) {
    // Initialize argparse
    argparse::ArgumentParser program("lstree", "1.0");
    // Define arguments with defaults
    program.add_argument("directory_path")
        .default_value(std::string("."))
        .help("Path to the directory to visualize. Defaults to the current directory.");
    program.add_argument("-x", "--x_spacing")
        .default_value(3)
        .scan<'i', int>() // Parse as integer
        .help("Horizontal spacing (number of spaces). Defaults to 3.");
    program.add_argument("-y", "--y_spacing")
        .default_value(1)
        .scan<'i', int>() // Parse as integer
        .help("Vertical spacing (number of lines). Defaults to 1.");
    // Parse arguments
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        std::cout << program;
        return 1;
    }

    // Retrieve parsed values
    std::string directory_path = program.get<std::string>("directory_path");
    int x_spacing = program.get<int>("--x_spacing");
    int y_spacing = program.get<int>("--y_spacing");

    // Initialize root level state
    level_states[0] = NO_VALUE;
    // Generate and print the directory hierarchy
    generate_directory_hierarchy(directory_path, x_spacing, y_spacing, 0);

    return 0;
}

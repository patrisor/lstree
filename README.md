# lstree

`lstree` is a powerful command-line tool for visualizing directory and file hierarchies in a tree-like structure. Inspired by the Unix `tree` command, `lstree` helps developers and users understand and navigate complex directory structures with ease.

## Features

- **Customizable Spacing:** Adjust horizontal (`x-spacing`) and vertical (`y-spacing`) spacing for cleaner visualization.
- **Sorting:** Enable or disable sorting of directory and file entries.
- **Ignore Files or Directories:** Specify files or directories to exclude from the output.
- **Recursive Directory Traversal:** Automatically explores and displays subdirectories.
- **Dynamic CLI:** Flexible argument parsing with default values for seamless use.

---

## Installation

### Prerequisites

- **C++20 Compiler:** Ensure you have a C++20 compatible compiler installed (e.g., `g++`, `clang++`).
- **Make:** Verify that `make` is installed on your system.

---

### Build and Install

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/lstree.git
   cd lstree
   ```

2. **Build the Executable:**

   ```bash
   make
   ```

3. **Deploy `lstree`:**

   ```bash
   ./deploy.sh
   ```

   This script will:
   - Move the executable to `/usr/local/bin`
   - Set executable permissions
   - Create an alias `lst` for simplified usage
   - Reload the shell for immediate use

---

## Usage

```bash
lstree [OPTIONS] [directory_path]
```

### **Options**

| Option                | Description                                                                 | Default          |
|-----------------------|-----------------------------------------------------------------------------|------------------|
| `directory_path`      | Path to the directory to visualize.                                        | Current directory |
| `-x, --x_spacing`     | Number of spaces for horizontal padding.                                   | 3                |
| `-y, --y_spacing`     | Number of lines for vertical padding.                                      | 1                |
| `-s, --sort`          | Enable (`true`) or disable (`false`) sorting of directory entries.         | `true`           |
| `-i, --ignore`        | List of file or directory names to exclude from the output.                | None             |

---

### **Examples**

#### **Default Usage**

Visualize the current directory structure:

```bash
lstree
```

#### **Custom Spacing**

Specify horizontal and vertical spacing:

```bash
lstree -x 5 -y 2
```

#### **Ignore Files or Directories**

Exclude `.git` and `node_modules` from the output:

```bash
lstree --ignore .git node_modules
```

#### **Disable Sorting**

Visualize the directory without sorting:

```bash
lstree --sort=false
```

---

### **Sample Output**

Command:
```bash
lstree tests/ -x 3 -y 1 --ignore .git .gitignore
```

Output:
```
tests/
├───subdir1/
│   ├───file1.txt
│   └───file2.txt
└───file3.txt

2 directories, 3 files
```

---

## Contributing

We welcome contributions! Feel free to open an issue or submit a pull request for new features, bug fixes, or improvements.

## License

MIT License. See [LICENSE](LICENSE) for details.

# lstree

`lstree` is a command-line tool that generates a visual hierarchy of directories and files, similar to the Unix `tree` command. It's designed to help visualize project structures, making it easier to understand and manage large codebases.

## Features

- **Customizable Spacing:** Adjust horizontal and vertical spacing for clear visualization.
- **Recursive Directory Traversal:** Automatically explores subdirectories.
- **Simple Integration:** Easily integrate into existing workflows.

## Installation

### Prerequisites

- **C++17 Compiler:** Ensure you have a C++17 compatible compiler installed (e.g., `g++`, `clang++`).
- **Make:** Make sure `make` is installed on your system.

### Build and Install

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/lstree.git
   cd lstree
   ```

2. **Build the Executable:**

   *(Assuming you have a `make.sh` script to build the project)*

   ```bash
   ./make.sh
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

## Usage

```bash
lstree <directory_path> <X_SPACING> <Y_SPACING>
```

- `<directory_path>`: Path to the target directory.
- `<X_SPACING>`: Number of spaces for horizontal padding.
- `<Y_SPACING>`: Number of lines for vertical padding.

**Example:**

```bash
lstree tests/ 3 1
```

**Output:**

```
tests/
│
└───test1/
    │
    ├───dist/
    │   │
    │   ├───css/
    │   │
    │   ├───js/
    │   │
    │   └───fonts/
    │
    ├───less/
    │
    ├───js/
    │
    ├───docs/
    │   │
    │   └───examples/
    │
    └───fonts/
```

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## License

MIT License. See [LICENSE](LICENSE) for details.
#!/bin/bash

# Deploy script for lstree
# Automates the installation of lstree globally

# Variables
EXECUTABLE="lstree"
INSTALL_DIR="/usr/local/bin"
ALIAS_NAME="lst"
SHELL_CONFIG="$HOME/.zshrc" # Change to .bashrc if using bash

# Ensure the executable exists locally
if [[ ! -f "$EXECUTABLE" ]]; then
  echo "Error: $EXECUTABLE not found. Run 'make' first."
  exit 1
fi

# Move the executable to the INSTALL_DIR
echo "Moving $EXECUTABLE to $INSTALL_DIR..."
sudo mv "$EXECUTABLE" "$INSTALL_DIR"
if [[ $? -ne 0 ]]; then
  echo "Error: Failed to move $EXECUTABLE to $INSTALL_DIR."
  exit 1
fi

# Set executable permissions
echo "Setting executable permissions..."
sudo chmod +x "$INSTALL_DIR/$EXECUTABLE"
if [[ $? -ne 0 ]]; then
  echo "Error: Failed to set executable permissions for $EXECUTABLE."
  exit 1
fi

# Add alias to shell config
echo "Creating alias '$ALIAS_NAME' for $EXECUTABLE..."
if ! grep -Fxq "alias $ALIAS_NAME='$EXECUTABLE'" "$SHELL_CONFIG"; then
  echo "alias $ALIAS_NAME='$EXECUTABLE'" >> "$SHELL_CONFIG"
else
  echo "Alias '$ALIAS_NAME' already exists in $SHELL_CONFIG."
fi

# Prompt user to reload shell configuration
echo
echo "Please reload your shell configuration manually by running:"
echo "source $SHELL_CONFIG"
echo "$EXECUTABLE installed successfully! Use '$ALIAS_NAME' to run it."
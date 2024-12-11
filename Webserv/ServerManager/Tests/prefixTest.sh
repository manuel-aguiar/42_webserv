#!/bin/bash

# Sponsored by ChatGPT, usage: ./prefixTest.sh [target folder path]

# Check if the target folder is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <target-folder>"
    exit 1
fi

TARGET_DIR="$1"

# Check if the target folder exists
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Target folder '$TARGET_DIR' does not exist."
    exit 1
fi

# Function to rename files and folders with "Test" prefix
rename_with_prefix() {
    local CURRENT_DIR="$1"
    for ITEM in "$CURRENT_DIR"/*; do
        # Skip if the item does not exist (handles empty directories)
        [ -e "$ITEM" ] || continue

        BASENAME=$(basename "$ITEM")
        PARENT_DIR=$(dirname "$ITEM")
        NEW_NAME="$PARENT_DIR/Test$BASENAME"

        # Rename the item
        mv "$ITEM" "$NEW_NAME"

        # If it's a directory, recursively rename its contents
        if [ -d "$NEW_NAME" ]; then
            rename_with_prefix "$NEW_NAME"
        fi
    done
}

# Start renaming from the target directory
rename_with_prefix "$TARGET_DIR"

echo "All files and subfolders in '$TARGET_DIR' have been prefixed with 'Test'."
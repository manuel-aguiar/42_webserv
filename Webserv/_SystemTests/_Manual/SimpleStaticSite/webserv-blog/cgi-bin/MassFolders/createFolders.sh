#!/bin/bash

# Define the parent directory (optional)
PARENT_DIR="folders"

SCRIPT_DIR = "$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$SCRIPT_DIR/$PARENT_DIR"

# Create 10,000 directories
for i in $(seq -w 1 10000); do
    mkdir -p "$SCRIPT_DIR/$PARENT_DIR/folder_$i"
done

echo "10,000 folders created in $SCRIPT_DIR/$PARENT_DIR."


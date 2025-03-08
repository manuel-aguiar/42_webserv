#!/bin/bash

# Define the parent directory (optional)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

PARENT_DIR="folders"
mkdir -p "$SCRIPT_DIR/$PARENT_DIR"

# Create 10,000 directories
for i in $(seq -w 1 10000); do
    mkdir -p "$SCRIPT_DIR/$PARENT_DIR/folder_$i"
done

echo "10,000 folders created in $SCRIPT_DIR/$PARENT_DIR."

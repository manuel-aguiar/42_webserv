#!/bin/bash

# Define the parent directory (optional)
PARENT_DIR="folders"
mkdir -p "$PARENT_DIR"

# Create 10,000 directories
for i in $(seq -w 1 10000); do
    mkdir -p "$PARENT_DIR/folder_$i"
done

echo "10,000 folders created in $PARENT_DIR."
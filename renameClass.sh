#!/bin/bash

# Ensure exactly three arguments are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <CurrentClassName> <NewClassName> <StartingFolder>"
    exit 1
fi

# Assign arguments to variables
OLD_CLASS="$1"
NEW_CLASS="$2"
START_DIR="$3"

# Ensure the starting folder exists
if [ ! -d "$START_DIR" ]; then
    echo "Error: The starting folder '$START_DIR' does not exist."
    exit 1
fi

echo "Renaming class from '$OLD_CLASS' to '$NEW_CLASS' in '$START_DIR'..."

# Step 1: Replace all occurrences in file contents (includes Makefile, .cpp, .hpp)
find "$START_DIR" -type f -name "*.cpp" -o -name "*.hpp" -o -name "*Makefile" | xargs sed -i "s/$OLD_CLASS/$NEW_CLASS/g"


# Step 2: Rename directories that contain the old class name
find "$START_DIR" -depth -type d -name "*$OLD_CLASS*" | while read -r dir; do
    newdir=$(echo "$dir" | sed "s/$OLD_CLASS/$NEW_CLASS/g")
    mv "$dir" "$newdir"
done

# Step 3: Rename files that contain the old class name
find "$START_DIR" -type f -name "*$OLD_CLASS*" | while read -r file; do
    newfile=$(echo "$file" | sed "s/$OLD_CLASS/$NEW_CLASS/g")
    mv "$file" "$newfile"
done

# Success message
echo "Success! Changed class name from '$OLD_CLASS' to '$NEW_CLASS', starting at folder '$START_DIR'."

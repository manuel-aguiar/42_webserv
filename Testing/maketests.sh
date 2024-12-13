#!/bin/bash

#
# Usage: ./maketests.sh [target folder path]
# 
# Creates a directory inside your directory structure for testing
# It assumes the Directory is named after the class and there is
# an [target folder path].hpp file to be included in test.cpp
# this is generic and not an all round testing builder
#


if [ -z "$1" ]; then
    echo "Usage: $0 <target-folder>"
    exit 1
fi

TARGET_DIR="$1"

TEST_DIR="_Tests"
TEST_FILE="test"
TESTDEPENDENCIES="TestDependencies"
MAKEFILE_FILE="Makefile"

# Check if the target folder exists
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Target folder '$TARGET_DIR' does not exist."
    exit 1
fi

createTestFile() {
	cat << EOF > "$1"/"$2"".cpp"


# include "../$3.hpp"
# include "$TESTDEPENDENCIES.hpp"
# include <iostream>

int main(void)
{

	try
	{
		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	return (0);
}

EOF
}

createTestDependencies() {
	cat << EOF > "$1"/"$2"".hpp"
#ifndef TESTDEPENDENCIES_HPP

# define TESTDEPENDENCIES_HPP

class Class
{

};


#endif
EOF
}

createMakefile() {
    cat << 'EOF' > "$1"/"$2"

# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

fclean: clean
	$(TARGET)

re: fclean all

.PHONY: clean fclean run
EOF
}


createTestFolder() {
    local CURRENT_DIR="$1"
    local TEST_FOLDER="$CURRENT_DIR/$TEST_DIR"

    if [ ! -d "$TEST_FOLDER" ]; then
        mkdir -p "$TEST_FOLDER"
    fi

	if [ ! -f "$TEST_FOLDER/$TEST_FILE" ]; then
		createTestFile "$TEST_FOLDER" "$TEST_FILE" "$(basename "$CURRENT_DIR")"
	fi
	if [ ! -f "$TEST_FOLDER/$TESTDEPENDENCIES" ]; then
		createTestDependencies "$TEST_FOLDER" "$TESTDEPENDENCIES"
	fi
	if [ ! -f "$TEST_FOLDER/$MAKEFILE_FILE" ]; then
		createMakefile "$TEST_FOLDER" "$MAKEFILE_FILE"
	fi		
    for ITEM in "$CURRENT_DIR"/*; do

        [ -e "$ITEM" ] || continue
		if [ -d "$ITEM" ] && [ "$(basename "$ITEM")" != "$TEST_DIR" ]; then
			createTestFolder "$ITEM"
		fi
		
    done
}

createTestFolder "$TARGET_DIR"

echo "All subfolders in '$TARGET_DIR' have a test folder."
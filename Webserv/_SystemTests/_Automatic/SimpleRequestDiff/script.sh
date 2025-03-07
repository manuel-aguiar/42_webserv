#!/bin/bash

# Server address
URL="http://localhost:8080"

# Function to run a test
test_case() {
    local test_name="$1"
    local request_type="$2"
    local resource="$3"
    local expected_file="$4"
    local actual_file="output_$test_name.txt"

    # Perform request
    curl -s -X "$request_type" "$URL$resource" -o "$actual_file"
    
    # Compare output
    if diff "$actual_file" "$expected_file" > /dev/null; then
        echo "PASS: $test_name"
    else
        echo "FAIL: $test_name"
    fi

    # Cleanup
    rm "$actual_file"
}

# Define tests
test_case "test1" "GET" "/"			"../_Testfiles/index.html"
test_case "test2" "GET" "/upload"	"../_Testfiles/upload/index.html"

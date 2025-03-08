#!/bin/bash

# Path to your webserver executable (adjust this)
WEBSERVER="../../../../webserv"
# Path to your config file (adjust this)
CONFIG_FILE="../SimpleRequestDiff1.conf"

# Array of test scripts to run
TEST_SCRIPTS=(
    "script.sh"
    # "OtherTest/script.sh"
    # "AnotherTest/script.sh"
)

# Function to cleanup processes on exit
cleanup() {
    echo "Cleaning up..."
    kill $WEBSERVER_PID 2>/dev/null
    exit
}

# Set up trap to catch SIGINT (Ctrl+C) and SIGTERM
trap cleanup SIGINT SIGTERM

# Start the webserver in background
$WEBSERVER $CONFIG_FILE &
WEBSERVER_PID=$!

# Wait a moment for the server to start up
sleep 2

# Check if webserver is running
if ! ps -p $WEBSERVER_PID > /dev/null; then
    echo "Error: Webserver failed to start"
    cleanup
    exit 1
fi

# Run all test scripts
FINAL_RESULT=0
for script in "${TEST_SCRIPTS[@]}"; do
    echo "----------------------------------------"
    echo "Running: $script"
    echo "----------------------------------------"
    bash "$script"
    TEST_RESULT=$?
    
    # If any test fails, mark the final result as failed
    if [ $TEST_RESULT -ne 0 ]; then
        FINAL_RESULT=1
    fi
done

# Cleanup and exit with final result
cleanup
exit $FINAL_RESULT
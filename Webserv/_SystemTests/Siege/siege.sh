#!/bin/bash

# Get the script's directory (absolute path)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Define absolute paths based on the script's location
SERVER_BINARY="$SCRIPT_DIR/../../../webserv"
CONFIG_FILE="$SCRIPT_DIR/SiegeFight.conf"
URLS_FILE="$SCRIPT_DIR/urls.txt"
SIEGE_LOG="$SCRIPT_DIR/siege_results.log"
SERVER_LOG="$SCRIPT_DIR/server.log"

RUN_TIME="10s"  # Duration for siege
CONCURRENCY="100" # Concurrent users for siege

make debug -C $SCRIPT_DIR/../../../
# Start the web server
echo "Starting web server..."
valgrind --track-fds=yes $SERVER_BINARY $CONFIG_FILE > $SERVER_LOG 2>&1 &
SERVER_PID=$!

# Give server time to start
sleep 2  # Adjust if needed

# Check if server is running
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "Web server failed to start. Check logs."
    exit 1
fi

echo "Web server started with PID $SERVER_PID"

# Run Siege tests in parallel
echo "Starting Siege load test..."
siege -c$CONCURRENCY -b -t$RUN_TIME -f $URLS_FILE --log=$SIEGE_LOG

# Capture siege exit code
SIEGE_EXIT_CODE=$?

# Stop the server after tests
echo "Stopping web server..."
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null

echo "Web server stopped."

# Check if Siege had issues
if [ $SIEGE_EXIT_CODE -ne 0 ]; then
    echo "Siege encountered errors. Check logs: $SIEGE_LOG"
    exit 1
fi

echo "Testing complete. Logs available:"
echo "  - Siege Log: $SIEGE_LOG"
echo "  - Server Log: $SERVER_LOG"
exit 0
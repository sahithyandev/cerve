#!/bin/bash

# Ensure the server is running before benchmarking!
# You might want to start your server here in a background process
# For example: ./your_server_executable &
# SERVER_PID=$!
# Give the server a moment to start
# sleep 1

echo "Running ApacheBench benchmarks..."

AB_RESULTS=$(ab -n 1000 -c 10 http://127.0.0.1:2000/ 2>&1)

echo "$AB_RESULTS"

echo "$AB_RESULTS" > benchmarks/ab_results.txt

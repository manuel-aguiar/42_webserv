#!/bin/bash

# Arrays for source files and corresponding program names
files=("mainNoPool.cpp" "mainalign.cpp" "mainalloc.cpp" "maindealloc.cpp")
programs=("nopool" "align" "alloc" "dealloc")

# Number of times to run each program
times=1000

# Number of repetitions to calculate average time
reps=50

# Clear the screen
clear

# Compilation loop
for i in "${!files[@]}"; do
  c++ -Wall -Wextra -Werror -std=c++98 "${files[i]}" -o "${programs[i]}"
done

# Execution loop
for prog in "${programs[@]}"; do
    echo -n "$prog: "

    # Initialize total time to 0 for averaging
    total_time=0

    # Run the program multiple times to calculate average time
    for ((i=1; i<=reps; i++)); do
        # Measure the execution time of the program
        exec_time=$( { /usr/bin/time -f "%e" ./$prog $times > /dev/null; } 2>&1 )
        total_time=$(echo "$total_time + $exec_time" | bc)
    done

    # Calculate the average time
    avg_time=$(echo "scale=3; $total_time / $reps" | bc)
    echo "Average time over $reps runs: $avg_time seconds"

    # Run the program one last time to generate the output file for diff comparison
    ./$prog $times > "$prog.txt"
    diff "$prog.txt" nopool.txt > "diff$prog.txt"

    # Check if diff file is empty (no differences)
    if [ ! -s "diff$prog.txt" ]; then
        rm -f "diff$prog.txt"   # Remove empty diff file
    else
        echo "Differences found in $prog compared to nopool:"
        cat "diff$prog.txt"     # Display diff content if differences exist
    fi

    # Clean up the generated output files (except nopool)
    if [ "$prog" != "nopool" ]; then
        rm -f "$prog.txt"
    fi
done

# Clean up the remaining nopool.txt and the last program binary
rm -f nopool.txt

for prog in "${programs[@]}"; do
    rm -f $prog
done
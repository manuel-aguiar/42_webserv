#!/bin/bash

# Arrays for source files and corresponding program names
files=("mainNoPool.cpp" "mainaligned.cpp" "mainalloc.cpp" "maindealloc.cpp")
programs=("nopool" "aligned" "alloc" "dealloc")

# Number of times to run each program
times=10000
clear
# Compilation loop
for i in "${!files[@]}"; do
  c++ -Wall -Wextra -Werror -std=c++98 "${files[i]}" -o "${programs[i]}"
done

# Clear the screen

# Execution loop
for prog in "${programs[@]}"; do
    echo -n "$prog:"
    time ./$prog $times > $prog.txt
    diff  $prog.txt  nopool.txt > "diff$prog.txt"
    cat "diff$prog.txt"
    echo ""

    if [ "$prog" != "nopool" ]; then
        rm -rf "$prog.txt"
    fi
    rm -rf "$prog"
    if [ ! -s "diff$prog.txt" ]; then
        rm -rf "diff$prog.txt"
    else
        cat "diff$prog.txt"
    fi


done

rm -rf nopool.txt "$prog"

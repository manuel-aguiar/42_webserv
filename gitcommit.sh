#!/bin/bash

if [ $# -eq 0 ]; then
	git add . && git commit -m "generic commit"
else
	git add . && git commit -m "$1"	
fi

#find /home/manuel/Desktop/42_webserv/ -type f \( -name "*.o" -o -name "*.d" -o -name "*.out" \) -exec rm -f {} +

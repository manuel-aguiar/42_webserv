#!/bin/bash

if [ $# -eq 0 ]; then
	git add . && git commit -m "generic commit"
else
	git add . && git commit -m "$1"
fi



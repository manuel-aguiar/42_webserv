#!/bin/bash

if [ $# -eq 0 ]; then
	git add . && git commit -m "generic commit" && git push
else
	git add . && git commit -m "$1" && git push	
fi



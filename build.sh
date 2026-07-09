#!/usr/bin/env bash

set -e

gcc -Wall \
	-Wextra \
	-Wpedantic \
	-O2 \
	main.c \
	-o ssSH

echo "build complete"

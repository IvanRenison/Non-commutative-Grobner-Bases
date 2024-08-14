#!/bin/bash

set -e

# Compile and run ach .cpp file
for file in ./test/*.cpp; do
  if [[ -f "$file" ]]; then
    g++ -Wall -Wextra -std=c++20 -O2 -o "${file%.cpp}.run" "$file"
    ./"${file%.cpp}.run"
  fi
done

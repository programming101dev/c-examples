#!/bin/bash

# Find and delete all files with the .out, .so, or .dylib extension
find . -name "*.out" -o -name "*.so" -o -name "*.dylib" -type f | while read -r file; do
  echo "Deleting $file"
  rm "$file"
  if [ $? -ne 0 ]; then
    echo "Deletion failed for $file. Exiting."
    exit 1
  fi
done

echo "Deletion complete."

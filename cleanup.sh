#!/bin/bash

# Find and delete all files with the .out extension
find . -name "*.out" -type f | while read -r file; do
  echo "Deleting $file"
  rm "$file"
  if [ $? -ne 0 ]; then
    echo "Deletion failed for $file. Exiting."
    exit 1
  fi
done

echo "Deletion complete."

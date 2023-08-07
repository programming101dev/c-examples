#!/bin/bash

# Find all directories that contain a.out files, sort them, and iterate through them
find . -name ".out" -exec dirname {} \; | sort -u | while read -r dir; do
  echo "Deleting a.out in directory: $dir"
  find "$dir" -maxdepth 1 -name ".out" -type f | sort | while read -r file; do
    rm "$file"
    if [ $? -ne 0 ]; then
      echo "Deletion failed for $file. Exiting."
      exit 1
    fi
  done
done

echo "Deletion complete."
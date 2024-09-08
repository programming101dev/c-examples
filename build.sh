#!/usr/bin/env bash

# Function to run make clean all in directories with Makefile
run_make() {
    local current_dir="$1"

    # Check if the directory name ends with ".dSYM" and skip it
    if [[ "$(basename "$current_dir")" == *".dSYM" ]]; then
        return
    fi

    echo -e "\nProcessing directory: $current_dir"

    # Check if a Makefile exists in the current directory
    if [ -f "$current_dir/Makefile" ]; then
        echo "Running 'make all' in $current_dir..."
        (cd "$current_dir" && make all)
        result=$?
        if [ $result -ne 0 ]; then
            echo "Error: Make failed in $current_dir."
            exit 1
        fi
    fi
}

# Function to traverse directories from the specified starting point
run_make_from_start_dir() {
    local start_dir="$1"
    local found_start=0

    # Find and sort directories alphabetically (use appropriate sorting method depending on OS)
    if [[ "$(uname)" == "Darwin" || "$(uname)" == "FreeBSD" ]]; then
        # For macOS and FreeBSD, sort using locale-aware sort
        all_dirs=($(find . -type d | LC_ALL=C sort))
    else
        # For Linux, plain sort should work
        all_dirs=($(find . -type d | sort))
    fi

    # Traverse through all directories
    for dir in "${all_dirs[@]}"; do
        # Skip directories before the start directory
        if [[ $found_start -eq 0 ]]; then
            if [[ "$dir" == "$start_dir" || "$dir" == "./$start_dir" ]]; then
                found_start=1
            else
                continue
            fi
        fi

        # Run make on the current directory
        run_make "$dir"
    done
}

# Parse command-line options
start_dir="." # Default to the current directory if -d is not provided

while getopts "d:" opt; do
    case $opt in
        d)
            start_dir="$OPTARG"
            ;;
        \?)
            echo "Usage: $0 -d <directory>"
            exit 1
            ;;
    esac
done

# Ensure .flags directory exists before running
if [ ! -d "./.flags" ]; then
  echo "You must run ./change-compiler.sh first"
  exit 1
fi

# Validate that the starting directory exists
if [ ! -d "$start_dir" ]; then
  echo "Error: Directory $start_dir does not exist."
  exit 1
fi

# Start the traversal process from the specified directory
run_make_from_start_dir "$start_dir"

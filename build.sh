#!/usr/bin/env bash

# Function to run make clean all in directories with Makefile
run_make() {
    local current_dir="$1"

    # Check if the directory name ends with ".dSYM" and skip it
    if [[ "$(basename "$current_dir")" == *".dSYM" ]]; then
        return
    fi

    echo -e "\nCompiling: $current_dir"

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

    # Find subdirectories and run make in them alphabetically
    local subdirs=()
    for subdir in "$current_dir"/*; do
        if [ -d "$subdir" ]; then
            subdirs+=("$subdir")
        fi
    done

    # Sort subdirectories alphabetically
    subdirs_sorted=($(printf '%s\n' "${subdirs[@]}" | sort))

    for subdir in "${subdirs_sorted[@]}"; do
        run_make "$subdir"
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

if [ ! -d "./.flags" ]; then
  echo "You must run ./change-compiler.sh first"
  exit 1
fi

# Start the traversal process from the specified directory
run_make "$start_dir"

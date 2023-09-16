#!/bin/bash

# Function to run make clean all in directories with Makefile
run_make() {
    local current_dir="$1"

    # Check if a Makefile exists in the current directory
    if [ -f "$current_dir/Makefile" ]; then
        echo "Running 'make clean' in $current_dir..."
        (cd "$current_dir" && make clean > /dev/null)  # Redirect stdout to /dev/null
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

# Start the traversal process from the current directory
run_make "."

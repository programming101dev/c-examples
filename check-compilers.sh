#!/usr/bin/env bash

# Exit the script if any command fails
set -e

# Initialize the list of potential compilers
c_compilers=("gcc" "clang" "gcc13" "gcc-13" "clang-15" "clang-16" "clang-17" "clang-18" "clang15" "clang16" "clang17" "clang18" "clang-devel")

# Function to check if a compiler exists and add it to the supported list
check_compiler()
{
    local compiler_type="$1"
    local compiler_list=("${@:2}")  # Get all arguments after the first one
    local output_filename="supported_${compiler_type}.txt"

    local supported_compilers=()  # Define the array here

    for compiler in "${compiler_list[@]}"; do
        if command -v "$compiler" &>/dev/null; then
            supported_compilers+=("$compiler")
        fi
    done

    # Output the supported compilers to a file
    {
        for compiler in "${supported_compilers[@]}"; do
            echo "$compiler"
        done
    } > "$output_filename"

    if [ $? -eq 0 ]; then
        echo "Supported $compiler_type compilers have been written to $output_filename"
    else
        echo "Failed to write supported $compiler_type compilers to $output_filename"
        exit 1
    fi
}

check_compiler "c_compilers" "${c_compilers[@]}"

exit 0

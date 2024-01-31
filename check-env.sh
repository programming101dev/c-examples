#!/usr/bin/env bash

# Exit the script if any command fails
set -e

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"

usage()
{
    echo "Usage: $0 -c <C compiler> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>]"
    echo "  -c c compiler     Specify the c compiler name (e.g. gcc or clang)"
    echo "  -f clang-format   Specify the clang-format name (e.g. clang-tidy or clang-tidy-17)"
    echo "  -t clang-tidy     Specify the clang-tidy name (e.g. clang-tidy or clang-tidy-17)"
    echo "  -k cppcheck       Specify the cppcheck name (e.g. cppcheck)"
    exit 1
}

# Parse command-line options using getopt
while getopts ":c:f:t:k:" opt; do
  case $opt in
    c)
      c_compiler="$OPTARG"
      ;;
    f)
      clang_format_name="$OPTARG"
      ;;
    t)
      clang_tidy_name="$OPTARG"
      ;;
    k)
      cppcheck_name="$OPTARG"
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      usage
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      usage
      ;;
  esac
done

# Check if the compiler argument is provided
if [ -z "$c_compiler" ]; then
  echo "Error: c compiler argument (-c) is required."
  usage
fi

# Function to check if a tool is in the PATH
check_tool() {
    if command -v "$1" >/dev/null 2>&1; then
        echo "$1 found in PATH"
        return 0
    else
        echo "$1 not found in PATH"
        return 1
    fi
}

# List of tools to check
tools=("cmake" "$c_compiler" "$clang_format_name" "$clang_tidy_name" "$cppcheck_name")

# Initialize a counter for missing tools
missing_count=0

# Loop through the list of tools
for tool in "${tools[@]}"; do
    check_tool "$tool" || ((missing_count++))
done

# Return the count of missing tools
echo "Total missing tools: $missing_count"

# Exit with the count of missing tools as the status code
exit "$missing_count"

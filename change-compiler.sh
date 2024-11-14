#!/usr/bin/env bash

set -e

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"
sanitizers="address,leak,pointer_overflow,undefined"

# Function to display script usage
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
while getopts ":c:f:t:k:s:" opt; do
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
    s)
      sanitizers="$OPTARG"
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

./check-env.sh -c "$c_compiler" -f "$clang_format_name" -t "$clang_tidy_name" -k "$cppcheck_name"

if [ ! -f "supported_c_compilers.txt" ] || ! grep -Fxq "$c_compiler" supported_c_compilers.txt; then
   ./check-compilers.sh
fi

if [ ! -d "./.flags/$c_compiler" ]; then
    ./generate-flags.sh
fi

# Function to determine the platform and set the shared library extension accordingly
get_shared_lib_extension() {
    case "$(uname -s)" in
        Linux)
            SHARED_EXT=".so"
            BINARY_EXT="" ;;
        Darwin)
            SHARED_EXT=".dylib"
            BINARY_EXT=".dSYM" ;;
        FreeBSD)
            SHARED_EXT=".so"
            BINARY_EXT="" ;;
        *)
            echo "Unsupported platform"
            exit 1
            ;;
    esac
}

# Function to generate the Makefile in the current directory
generate_makefile()
{
    # Determine the library names based on the platform
    # Define LIBRARIES based on the platform
    case "$(uname -s)" in
        Linux) LIBRARIES="" ;;
        Darwin) LIBRARIES="" ;;
        FreeBSD)
            # Check if the directory name starts with "pthread"
            if [[ "$(basename "$(pwd)")" == pthread* ]]; then
                LIBRARIES="-lpthread"
            else
                LIBRARIES=""
            fi
            ;;
        *)
            echo "Unsupported platform"
            exit 1
            ;;
    esac

    # Initialize the Makefile
    echo -e ".PHONY: main main-traceable clean lint all\n" > Makefile
    echo -e "CC=$c_compiler" >> Makefile
    echo -e "COMPILATION_FLAGS=-std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE -D_GNU_SOURCE -D__BSD_VISIBLE -Werror" >> Makefile
    echo -e "SUPPORTED_ANALYZER_FLAGS=$SUPPORTED_ANALYZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_CODE_GENERATION_FLAGS=$SUPPORTED_CODE_GENERATION_FLAGS" >> Makefile
    echo -e "SUPPORTED_DEBUG_FLAGS=$SUPPORTED_DEBUG_FLAGS" >> Makefile
    echo -e "SUPPORTED_INSTRUMENTATION_FLAGS=$SUPPORTED_INSTRUMENTATION_FLAGS" >> Makefile
    echo -e "SUPPORTED_OPTIMIZATION_FLAGS=$SUPPORTED_OPTIMIZATION_FLAGS" >> Makefile
    echo -e "SUPPORTED_WARNING_FLAGS=$SUPPORTED_WARNING_FLAGS" >> Makefile
    echo -e "SUPPORTED_ADDRESS_SANITIZER_FLAGS=$SUPPORTED_ADDRESS_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_CFI_SANITIZER_FLAGS=$SUPPORTED_CFI_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_DATAFLOW_SANITIZER_FLAGS=$SUPPORTED_DATAFLOW_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_HWADDRESS_SANITIZER_FLAGS=$SUPPORTED_HWADDRESS_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_LEAK_SANITIZER_FLAGS=$SUPPORTED_LEAK_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_MEMORY_SANITIZER_FLAGS=$SUPPORTED_MEMORY_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_POINTER_OVERLFLOW_SANITIZER_FLAGS=$SUPPORTED_POINTER_OVERLFLOW_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$SUPPORTED_SAFE_STACK_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_SHADOW_STACK_SANITIZER_FLAGS=$SUPPORTED_SHADOW_STACK_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_THREAD_SANITIZER_FLAGS=$SUPPORTED_THREAD_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_UNDEFINED_SANITIZER_FLAGS=$SUPPORTED_UNDEFINED_SANITIZER_FLAGS" >> Makefile

    echo -e "CLANG_TIDY_CHECKS=-checks=*,-llvmlibc-restrict-system-libc-headers,-altera-struct-pack-align,-readability-identifier-length,-altera-unroll-loops,-cppcoreguidelines-init-variables,-cert-err33-c,-modernize-macro-to-enum,-bugprone-easily-swappable-parameters,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-altera-id-dependent-backward-branch,-concurrency-mt-unsafe,-misc-unused-parameters,-hicpp-signed-bitwise,-google-readability-todo,-cert-msc30-c,-cert-msc50-cpp,-readability-function-cognitive-complexity,-clang-analyzer-security.insecureAPI.strcpy,-cert-env33-c,-android-cloexec-accept,-clang-analyzer-security.insecureAPI.rand,-misc-include-cleaner,-cppcoreguidelines-macro-to-enum" >> Makefile
    echo -e "LIBRARIES=${LIBRARIES}" >> Makefile
    echo -e "PROGRAMS=" >> Makefile

    if [ -n "$BINARY_EXT" ]; then
        echo -e "BINARIES=" >> Makefile
    fi

    echo "LIBS=" >> Makefile
    echo -e "SOURCES=\n" >> Makefile

    # Loop through each .c file in the current directory
    for file in *.c; do
        if [[ -f "$file" ]]; then
            # Extract the file name without extension
            filename="${file%.c}"

            # Check if the file is one of the testlib files
            if [[ "$filename" == "testlib-1" || "$filename" == "testlib-2" ]]; then
                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(CODE_GENERATION_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -shared -fPIC -I/usr/local/include -o lib$filename$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo -e "LIBS += lib$filename$SHARED_EXT" >> Makefile

                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename-traceaable$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(CODE_GENERATION_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -shared -fPIC -I/usr/local/include -o lib$filename-traceaable$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo -e "LIBS += lib$filename-traceaable$SHARED_EXT\n" >> Makefile
            else
                echo "SOURCES += $filename.c" >> Makefile

                # Generate an executable rule with the supported warning flags
                echo "$filename: $file" >> Makefile
                echo -e "\t@echo \"Compiling $file -> $filename\"" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(CODE_GENERATION_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o $filename $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename" >> Makefile
                echo -e "BINARIES += $filename$BINARY_EXT\n" >> Makefile

                # Generate a traceable version rule
                echo -e "$filename-traceable: $file" >> Makefile
                echo -e "\t@echo \"Compiling $file -> $filename-traceable\"" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(CODE_GENERATION_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o $filename-traceable $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename-traceable" >> Makefile

                if [ -n "$BINARY_EXT" ]; then
                    echo -e "BINARIES += $filename-traceable$BINARY_EXT\n" >> Makefile
                fi
            fi
        fi
    done

    # Add a clean rule to reset coverage
    echo -e "cleanup:" >> Makefile
    echo -e "\t@rm -rf *.dSYM" >> Makefile
    echo -e "\t@rm -f *.gcda" >> Makefile
    echo -e "\t@rm -f *.gcno" >> Makefile

    # Add a format rule to format the source code
    echo -e "format:" >> Makefile
    echo -e "\t@echo \"Formatting source code...\"" >> Makefile
	  echo -e "\t@$clang_format_name -i --style=file \$(SOURCES)" >> Makefile

    # Add a lint rule to pick on the source code
    echo -e "\nlint:" >> Makefile
    echo -e "\t@echo \"Running clang-tidy for static code analysis...\"" >> Makefile
	  echo -e "\t@$clang_tidy_name \$(SOURCES) -quiet --warnings-as-errors='*' \$(CLANG_TIDY_CHECKS) -- \$(COMPILATION_FLAGS) \$(CFLAGS) -I/usr/local/include" >> Makefile

    if [[ "$c_compiler" == *clang* ]]; then
        # Add an analyzer rule to pick on the source code
        echo -e "\nanalyze:" >> Makefile
        echo -e "\t@echo \"Running $CC for static code analysis...\"" >> Makefile
        echo -e "\t@\${CC} --analyze --analyzer-output text -Xclang -analyzer-checker=core --analyze -Xclang -analyzer-checker=deadcode -Xclang -analyzer-checker=security -Xclang -analyzer-disable-checker=security.insecureAPI.DeprecatedOrUnsafeBufferHandling -Xclang -analyzer-disable-checker=security.insecureAPI.rand -Xclang -analyzer-checker=unix -I/usr/local/include \$(CFLAGS) \$(COMPILATION_FLAGS) \$(SOURCES)" >> Makefile
    fi

    # Add a check rule to pick on the source code
    echo -e "\ncheck:" >> Makefile
    echo -e "\t@echo \"Running cppcheck for static code analysis...\"" >> Makefile
    echo -e "\t@$cppcheck_name  -D__cppcheck__ --error-exitcode=1 --force --quiet --inline-suppr --check-level=exhaustive --library=posix --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression --suppress=checkersReport -I/usr/local/include \${SOURCES}" >> Makefile

    # Add a clean rule to be picky about the code
    echo -e "\nclean:" >> Makefile
    echo -e "\t@rm -f \$(PROGRAMS) \$(LIBS)" >> Makefile

    if [ -n "$BINARY_EXT" ]; then
        echo -e "\t@rm -rf \$(BINARIES)" >> Makefile
    fi

    echo -e "\t@rm -rf *.dSYM" >> Makefile
    echo -e "\t@rm -f *.gcda" >> Makefile
    echo -e "\t@rm -f *.gcno" >> Makefile

    # Add an "all" rule to build all programs and libraries
    if [[ "$c_compiler" == *clang* ]]; then
        echo -e "\nall: cleanup format \$(PROGRAMS) \$(LIBS) lint analyze check" >> Makefile
    else
        echo -e "\nall: cleanup format \$(PROGRAMS) \$(LIBS) lint check" >> Makefile
    fi

    # Print a message indicating the Makefile generation is complete
    echo -e "\nMakefile generated successfully in directory: $(pwd)"
}

# Function to process directories recursively
process_directories()
{
    local dir="$1"

    # Loop through each subdirectory
    for subdir in "$dir"/*/; do
        if [[ -d "$subdir" ]]; then
            echo "$subdir"
            # Check if the directory is not tracked by Git
            if ! git ls-files --error-unmatch "$subdir" >/dev/null 2>&1; then
                # Delete the directory and its contents
                rm -rf "$subdir"
                echo "Deleted '$subdir'"
            else
                # Check if there are .c files in the subdirectory
                if [[ -n $(find "$subdir" -maxdepth 1 -name "*.c" -print -quit) ]]; then
                    # Generate Makefile in the subdirectory
                    (cd "$subdir" && generate_makefile)
                fi
            fi

            # Recursively process subdirectories
            process_directories "$subdir"
        fi
    done
}

# Main script execution starts here

# Echo the value of the CC variable
echo "c_compiler is set to: $c_compiler"

if [ ! -d "./.flags" ]; then
    ./generate-flags.sh
fi

SUPPORTED_ADDRESS_SANITIZER_FLAGS=
SUPPORTED_CFI_SANITIZER_FLAGS=
SUPPORTED_DATAFLOW_SANITIZER_FLAGS=
SUPPORTED_HWADDRESS_SANITIZER_FLAGS=
SUPPORTED_MEMORY_SANITIZER_FLAGS=
SUPPORTED_POINTER_OVERLFLOW_SANITIZER_FLAGS=
SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=
SUPPORTED_THREAD_SANITIZER_FLAGS=
SUPPORTED_UNDEFINED_SANITIZER_FLAGS=

# Split the sanitizers string and construct flags
IFS=',' read -ra SANITIZERS <<< "$sanitizers"
for sanitizer in "${SANITIZERS[@]}"; do
    if [[ $sanitizer == "address" ]]; then
      SUPPORTED_ADDRESS_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/address_sanitizer_flags.txt")
    elif [[ $sanitizer == "cfi" ]]; then
      SUPPORTED_CFI_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/cfi_sanitizer_flags.txt")
    elif [[ $sanitizer == "dataflow" ]]; then
      SUPPORTED_DATAFLOW_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/dataflow_sanitizer_flags.txt")
    elif [[ $sanitizer == "hwaddress" ]]; then
      SUPPORTED_HWADDRESS_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/hwaddress_sanitizer_flags.txt")
    elif [[ $sanitizer == "leak" ]]; then
      SUPPORTED_MEMORY_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/leak_sanitizer_flags.txt")
    elif [[ $sanitizer == "memory" ]]; then
      SUPPORTED_MEMORY_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/memory_sanitizer_flags.txt")
    elif [[ $sanitizer == "pointer_overflow" ]]; then
      SUPPORTED_POINTER_OVERLFLOW_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/pointer_overflow_sanitizer_flags.txt")
    elif [[ $sanitizer == "safe_stack" ]]; then
      SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/safe_stack_sanitizer_flags.txt")
    elif [[ $sanitizer == "shadow_call_stack" ]]; then
      SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/shadow_call_stack_sanitizer_flags.txt")
    elif [[ $sanitizer == "thread" ]]; then
      SUPPORTED_THREAD_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/thread_sanitizer_flags.txt")
    elif [[ $sanitizer == "undefined" ]]; then
      SUPPORTED_UNDEFINED_SANITIZER_FLAGS=$(cat "./.flags/${c_compiler}/undefined_sanitizer_flags.txt")
    fi
done

SUPPORTED_ANALYZER_FLAGS=$(cat "./.flags/${c_compiler}/analyzer_flags.txt")
SUPPORTED_DEBUG_FLAGS=$(cat "./.flags/${c_compiler}/debug_flags.txt")
SUPPORTED_INSTRUMENTATION_FLAGS=$(cat "./.flags/${c_compiler}/instrumentation_flags.txt")
SUPPORTED_OPTIMIZATION_FLAGS=$(cat "./.flags/${c_compiler}/optimization_flags.txt")
SUPPORTED_WARNING_FLAGS=$(cat "./.flags/${c_compiler}/warning_flags.txt")

# Determine the shared library extension based on the platform
get_shared_lib_extension

# Start processing directories from the current directory
process_directories "."

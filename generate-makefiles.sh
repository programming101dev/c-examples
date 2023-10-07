#!/usr/bin/env bash

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
    echo -e "CC=$CC" >> Makefile
    echo -e "COMPILATION_FLAGS=-std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE -D_GNU_SOURCE -D__BSD_VISIBLE -Werror" >> Makefile
    echo -e "SUPPORTED_WARNING_FLAGS=$SUPPORTED_WARNING_FLAGS" >> Makefile
    echo -e "SUPPORTED_SANITIZER_FLAGS=$SUPPORTED_SANITIZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_ANALYZER_FLAGS=$SUPPORTED_ANALYZER_FLAGS" >> Makefile
    echo -e "SUPPORTED_DEBUG_FLAGS=$SUPPORTED_DEBUG_FLAGS" >> Makefile
    echo -e "CLANG_TIDY_CHECKS=-checks=*,-llvmlibc-restrict-system-libc-headers,-altera-struct-pack-align,-readability-identifier-length,-altera-unroll-loops,-cppcoreguidelines-init-variables,-cert-err33-c,-modernize-macro-to-enum,-bugprone-easily-swappable-parameters,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-altera-id-dependent-backward-branch,-concurrency-mt-unsafe,-misc-unused-parameters,-hicpp-signed-bitwise,-google-readability-todo,-cert-msc30-c,-cert-msc50-cpp,-readability-function-cognitive-complexity,-clang-analyzer-security.insecureAPI.strcpy,-cert-env33-c,-android-cloexec-accept,-clang-analyzer-security.insecureAPI.rand,-misc-include-cleaner" >> Makefile
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
            full_path=$(realpath "$file")
            last_dir=$(basename "$(dirname "$full_path")")
            second_to_last_dir=$(basename "$(dirname "$(dirname "$full_path")")")

            # Check if the file is one of the testlib files
            if [[ "$filename" == "testlib-1" || "$filename" == "testlib-2" ]]; then
                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) -shared -fPIC -I/usr/local/include -o lib$filename$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo -e "LIBS += lib$filename$SHARED_EXT" >> Makefile

                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename-traceaable$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) -Wno-strict-flex-arrays -shared -fPIC -I/usr/local/include -o lib$filename-traceaable$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo -e "LIBS += lib$filename-traceaable$SHARED_EXT\n" >> Makefile
            else
                if [[ "$second_to_last_dir/$last_dir/$file" == "memory/malloc-free/main.c" || "$second_to_last_dir/$last_dir/$file" == "memory/memset/main.c" ]]; then
                    if is_flag_supported "-Wno-analyzer-use-of-uninitialized-value"; then
                        echo -e "SUPPORTED_ANALYZER_FLAGS+=-Wno-analyzer-use-of-uninitialized-value" >> Makefile
                    fi
                    if is_flag_supported "-Wno-sometimes-uninitialized"; then
                        echo -e "SUPPORTED_ANALYZER_FLAGS+=-Wno-sometimes-uninitialized" >> Makefile
                     fi
                fi

                echo "SOURCES += $filename.c" >> Makefile

                # Generate an executable rule with the supported warning flags
                echo "$filename: $file" >> Makefile
                echo -e "\t@echo \"Compiling $file -> $filename\"" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) -I/usr/local/include -I/usr/local/include -o $filename $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename" >> Makefile
                echo -e "BINARIES += $filename$BINARY_EXT\n" >> Makefile

                # Generate a traceable version rule
                echo -e "$filename-traceable: $file" >> Makefile
                echo -e "\t@echo \"Compiling $file -> $filename-traceable\"" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) -Wno-strict-flex-arrays -I/usr/local/include -I/usr/local/include -o $filename-traceable $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename-traceable" >> Makefile

                if [ -n "$BINARY_EXT" ]; then
                    echo -e "BINARIES += $filename-traceable$BINARY_EXT\n" >> Makefile
                fi
            fi
        fi
    done

    # Add a format rule to format the source code
    echo -e "format:" >> Makefile
    echo -e "\t@echo \"Formatting source code...\"" >> Makefile
	  echo -e "\t@clang-format -i --style=file \$(SOURCES)" >> Makefile

    # Add a lint rule to pick on the source code
    echo -e "\nlint:" >> Makefile
    echo -e "\t@echo \"Running clang-tidy for static code analysis...\"" >> Makefile
	  echo -e "\t@clang-tidy \$(SOURCES) -quiet --warnings-as-errors='*' \$(CLANG_TIDY_CHECKS) -- \$(COMPILATION_FLAGS) \$(CFLAGS) -I/usr/local/include" >> Makefile

    if [[ "$CC" == *clang* ]]; then
        # Add an analyzer rule to pick on the source code
        echo -e "\nanalyze:" >> Makefile
        echo -e "\t@echo \"Running $CC for static code analysis...\"" >> Makefile
        echo -e "\t@\${CC} --analyze --analyzer-output text -Xclang -analyzer-checker=core --analyze -Xclang -analyzer-checker=deadcode -Xclang -analyzer-checker=security -Xclang -analyzer-disable-checker=security.insecureAPI.DeprecatedOrUnsafeBufferHandling -Xclang -analyzer-checker=unix -Xclang -analyzer-checker=unix -I/usr/local/include \$(CFLAGS) \$(COMPILATION_FLAGS) \$(SOURCES)" >> Makefile
    fi

    # Add a check rule to pick on the source code
    echo -e "\ncheck:" >> Makefile
    echo -e "\t@echo \"Running cppcheck for static code analysis...\"" >> Makefile
    echo -e "\t@cppcheck --error-exitcode=1 --force --quiet --inline-suppr --library=posix --enable=all --suppress=missingIncludeSystem --suppress=ConfigurationNotChecked --suppress=unmatchedSuppression -I/usr/local/include \${SOURCES}" >> Makefile

    # Add a clean rule to be picky about the code
    echo -e "\nclean:" >> Makefile
    echo -e "\t@rm -f \$(PROGRAMS) \$(LIBS)" >> Makefile

    if [ -n "$BINARY_EXT" ]; then
        echo -e "\t@rm -rf \$(BINARIES)" >> Makefile
    fi

    # Add an "all" rule to build all programs and libraries
    if [[ "$CC" == *clang* ]]; then
        echo -e "\nall: format \$(PROGRAMS) \$(LIBS) lint analyze check" >> Makefile
    else
        echo -e "\nall: format \$(PROGRAMS) \$(LIBS) lint check" >> Makefile
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
            # Check if there are .c files in the subdirectory
            if [[ -n $(find "$subdir" -maxdepth 1 -name "*.c" -print -quit) ]]; then
                # Generate Makefile in the subdirectory
                (cd "$subdir" && generate_makefile)
            fi

            # Recursively process subdirectories
            process_directories "$subdir"
        fi
    done
}

# Main script execution starts here

# Echo the value of the CC variable
echo "CC is set to: $CC"

SUPPORTED_WARNING_FLAGS=$(cat "$CC"_warning_flags.txt)
SUPPORTED_SANITIZER_FLAGS=$(cat "$CC"_sanitizer_flags.txt)
SUPPORTED_ANALYZER_FLAGS=$(cat "$CC"_analyzer_flags.txt)
SUPPORTED_DEBUG_FLAGS=$(cat "$CC"_debug_flags.txt)

# Determine the shared library extension based on the platform
get_shared_lib_extension

# Call the function to populate the supported flags
populate_supported_flags

# Start processing directories from the current directory
process_directories "."

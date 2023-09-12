#!/usr/bin/env bash

# Flags to test and use
WARNING_FLAGS=(
        "-Werror"
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wshadow"
        "-Wstrict-overflow=4"
        "-Wswitch-default"
        "-Wswitch-enum"
        "-Wunused"
        "-Wunused-macros"
        "-Wdate-time"
        "-Winvalid-pch"
        "-Wmissing-declarations"
        "-Wmissing-include-dirs"
        "-Wmissing-prototypes"
        "-Wstrict-prototypes"
        "-Wundef"
        "-Wnull-dereference"
        "-Wstack-protector"
        "-Wdouble-promotion"
        "-Wvla"
        "-Walloca"
        "-Woverlength-strings"
        "-Wdisabled-optimization"
        "-Winline"
        "-Wcast-qual"
        "-Wfloat-equal"
        "-Wformat=2"
        "-Wfree-nonheap-object"
        "-Wshift-overflow"
        "-Wwrite-strings"
        "-Wunsuffixed-float-constants"
        "-Warith-conversion"
        "-Wunsafe-loop-optimizations"
        "-Wvector-operation-performance"
        "-Walloc-zero"
        "-Wtrampolines"
        "-Wtsan"
        "-Wformat-overflow=2"
        "-Wformat-signedness"
        "-Wjump-misses-init"
        "-Wformat-truncation=2"
)

SANITIZER_FLAGS=(
        "-fsanitize=pointer-compare"
        "-fsanitize=address"
        "-fsanitize=pointer-subtract"
        "-fsanitize=undefined"
        "-fsanitize-address-use-after-scope"
        "-fcf-protection=full"
        "-fharden-compares"
        "-fharden-conditional-branches"
        "-fharden-control-flow-redundancy"
        "-fstack-protector=all"
        "-fstack-clash-protection"
        "-finstrument-functions"
        "-fsanitize=leak"
        "-fno-delete-null-pointer-checks"
        "-fno-omit-frame-pointer"
)

ANALYZER_FLAGS=(
        "-fanalyzer"
        "-fanalyzer-call-summaries"
        "-fanalyzer-checker=name"
        "-fanalyzer-fine-grained"
        "-fanalyzer-show-events-in-system-headers"
        "-fanalyzer-transitivity"
        "-fanalyzer-verbose-edges"
        "-fanalyzer-verbose-state-changes"
        "-fanalyzer-verbosity=4"
        "-fdump-analyzer"
        "-fdump-analyzer-callgraph"
        "-fdump-analyzer-exploded-graph"
        "-fdump-analyzer-exploded-nodes-3"
        "-fdump-analyzer-exploded-paths"
        "-fdump-analyzer-feasibility"
        "-fdump-analyzer-json"
        "-fdump-analyzer-state-purge"
        "-fdump-analyzer-stderr"
        "-fdump-analyzer-supergraph"
        "-fdump-analyzer-untracked"
        "-Wanalyzer-too-complex"
)

DEBUG_FLAGS=(
        "-g3"
        "-ggdb"
        "-fvar-tracking"
        "-fvar-tracking-assignments"
        "-gcolumn-info"
)

# Check if the provided flag is supported
function is_flag_supported() {
    local flag="$1"
    echo "int main(void) { return 0; }" > test.c
    clang -Werror=unknown-warning-option $flag test.c -o test.out 2>/dev/null
    local result=$?
    rm test.c test.out 2>/dev/null
    return $result
}

# Detect platform
PLATFORM=$(uname)

# Filter out unsupported flags
SUPPORTED_WARNING_FLAGS=""
for FLAG in "${WARNING_FLAGS[@]}"; do
    if is_flag_supported "$FLAG"; then
        SUPPORTED_WARNING_FLAGS="$SUPPORTED_WARNING_FLAGS $FLAG"
    fi
done

SUPPORTED_SANITIZER_FLAGS=""
ADDRESS_SUPPORTED=false
for FLAG in "${SANITIZER_FLAGS[@]}"; do
    if [ "$FLAG" == "-fsanitize=address" ]; then
        if is_flag_supported "$FLAG"; then
            ADDRESS_SUPPORTED=true
            SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
        fi
    elif [ "$FLAG" == "-fsanitize=pointer-subtract" ]; then
        if $ADDRESS_SUPPORTED && is_flag_supported "-fsanitize=address $FLAG"; then
            SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
        fi
    else
        if is_flag_supported "$FLAG"; then
            if [ "$FLAG" != "-fstack-clash-protection" ] || [ "$PLATFORM" != "Darwin" ]; then
                SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
            fi
        fi
    fi
done

SUPPORTED_ANALYZER_FLAGS=""
for FLAG in "${ANALYZER_FLAGS[@]}"; do
    if is_flag_supported "$FLAG"; then
        SUPPORTED_ANALYZER_FLAGS="$SUPPORTED_ANALYZER_FLAGS $FLAG"
    fi
done

SUPPORTED_DEBUG_FLAGS=""
for FLAG in "${DEBUG_FLAGS[@]}"; do
    if is_flag_supported "$FLAG"; then
        SUPPORTED_DEBUG_FLAGS="$SUPPORTED_DEBUG_FLAGS $FLAG"
    fi
done

# Generate the final script
cat <<EOL > build.sh
#!/usr/bin/env bash

set -e

WARNING_FLAGS="$SUPPORTED_WARNING_FLAGS"
SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS"
ANALYZER_FLAGS="$SUPPORTED_ANALYZER_FLAGS"
DEBUG_FLAGS="$SUPPORTED_DEBUG_FLAGS"

# Find all directories that contain .c files, sort them, and iterate through them
find . -name "*.c" -exec dirname {} \; | sort -u | while read -r dir; do
  echo "Compiling in directory: \$dir"
  find "\$dir" -maxdepth 1 -name "*.c" -type f | sort | while read -r file; do
    if [[ "\$file" == *"testlib-1.c" || "\$file" == *"testlib-2.c" ]]; then
      if [[ "\$(uname)" == "Darwin" ]]; then
        \$CC -std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE -D_GNU_SOURCE -shared -fPIC \$WARNING_FLAGS \$SANITIZER_FLAGS \$ANALYZER_FLAGS \$DEBUG_FLAGS "\$file" -o "\${file%.c}.dylib"
      else
        \$CC -std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE -D_GNU_SOURCE -shared -fPIC \$WARNING_FLAGS \$SANITIZER_FLAGS \$ANALYZER_FLAGS \$DEBUG_FLAGS "\$file" -o "\${file%.c}.so"
      fi
    else
      \$CC -std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE -D_GNU_SOURCE \$WARNING_FLAGS \$SANITIZER_FLAGS \$ANALYZER_FLAGS \$DEBUG_FLAGS "\$file" -o "\${file%.c}.out"
    fi

    if [ \$? -ne 0 ]; then
      echo "Compilation failed for \$file. Exiting."
      exit 1
    fi
  done
done

echo "Compilation complete."
EOL

chmod +x build.sh
echo "Generated script build.sh with supported flags."
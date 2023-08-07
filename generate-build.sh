#!/bin/bash

# Flags to test and use
WARNING_FLAGS=(
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
  "-fsanitize=address"
  "-fsanitize=pointer-compare"
  "-fsanitize=pointer-subtract"
  "-fsanitize=undefined"
  "-fsanitize-address-use-after-scope"
  "-fcf-protection=full"
  "-fharden-control-flow-redundancy"
  "-fstack-protector=all"
  "-finstrument-functions"
  "-fsanitize=leak"
  "-fno-delete-null-pointer-checks"
  "-fno-omit-frame-pointer"
)

# Variables to accumulate supported flags
SUPPORTED_WARNING_FLAGS=""
SUPPORTED_SANITIZER_FLAGS=""

# Minimal C program to test compile
echo "int main() { return 0; }" > test.c

# Test each warning flag
for FLAG in "${WARNING_FLAGS[@]}"; do
  clang -Werror=unknown-warning-option $FLAG test.c -o test.out 2>/dev/null
  if [ $? -eq 0 ]; then
    SUPPORTED_WARNING_FLAGS="$SUPPORTED_WARNING_FLAGS $FLAG"
  fi
done

# Test each sanitizer flag, taking dependencies into account
ADDRESS_SUPPORTED=false
for FLAG in "${SANITIZER_FLAGS[@]}"; do
  if [ "$FLAG" == "-fsanitize=address" ]; then
    clang -Werror=unknown-warning-option $FLAG test.c -o test.out 2>/dev/null
    if [ $? -eq 0 ]; then
      ADDRESS_SUPPORTED=true
      SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
    fi
  elif [ "$FLAG" == "-fsanitize=pointer-subtract" ]; then
    if $ADDRESS_SUPPORTED; then
      clang -Werror=unknown-warning-option "-fsanitize=address" $FLAG test.c -o test.out 2>/dev/null
      if [ $? -eq 0 ]; then
        SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
      fi
    fi
  else
    clang -Werror=unknown-warning-option $FLAG test.c -o test.out 2>/dev/null
    if [ $? -eq 0 ]; then
      SUPPORTED_SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS $FLAG"
    fi
  fi
done

# Clean up the test files
rm test.c test.out 2>/dev/null

# Generate the final script
cat <<EOL > build.sh
#!/bin/bash

WARNING_FLAGS="$SUPPORTED_WARNING_FLAGS"
SANITIZER_FLAGS="$SUPPORTED_SANITIZER_FLAGS"

# Find all directories that contain .c files, sort them, and iterate through them
find . -name "*.c" -exec dirname {} \; | sort -u | while read -r dir; do
  echo "Compiling in directory: \$dir"
  find "\$dir" -maxdepth 1 -name "*.c" -type f | sort | while read -r file; do
    if [[ "\$file" == *"testlib-1.c" || "\$file" == *"testlib-2.c" ]]; then
      if [[ "\$(uname)" == "Darwin" ]]; then
        gcc -shared -fPIC \$WARNING_FLAGS \$SANITIZER_FLAGS "\$file" -o "\${file%.c}.dylib"
      else
        gcc -shared -fPIC \$WARNING_FLAGS \$SANITIZER_FLAGS "\$file" -o "\${file%.c}.so"
      fi
    else
      gcc \$WARNING_FLAGS \$SANITIZER_FLAGS "\$file" -o "\${file%.c}.out"
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

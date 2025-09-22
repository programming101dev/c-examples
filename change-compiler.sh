#!/usr/bin/env bash
set -euo pipefail

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"
sanitizers=""

usage() {
  cat <<EOF
Usage: $0 -c <C compiler> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>] [-s <sanitizers>]
  -c c compiler     gcc, clang, clang-20, etc.
  -f clang-format   clang-format binary name
  -t clang-tidy     clang-tidy binary name
  -k cppcheck       cppcheck binary name
  -s sanitizers     comma list: address,undefined,thread,leak,memory,cfi,dataflow,hwaddress,pointer_overflow,safe_stack,shadow_call_stack
EOF
  exit 1
}

while getopts ":c:f:t:k:s:h" opt; do
  case "$opt" in
    c) c_compiler="$OPTARG" ;;
    f) clang_format_name="$OPTARG" ;;
    t) clang_tidy_name="$OPTARG" ;;
    k) cppcheck_name="$OPTARG" ;;
    s) sanitizers="$OPTARG" ;;
    h|\?|:) usage ;;
  esac
done

[[ -n "$c_compiler" ]] || { echo "Error: -c is required." >&2; usage; }

./check-env.sh -c "$c_compiler" -f "$clang_format_name" -t "$clang_tidy_name" -k "$cppcheck_name"
[[ -f supported_c_compilers.txt ]] || ./check-compilers.sh
grep -Fxq "$c_compiler" supported_c_compilers.txt || ./check-compilers.sh
[[ -d "./.flags/$c_compiler" ]] || ./generate-flags.sh

# Platform
SHARED_EXT=""; SHARED_LDFLAGS=""
case "$(uname -s)" in
  Linux|FreeBSD) SHARED_EXT=".so";    SHARED_LDFLAGS="-shared -fPIC" ;;
  Darwin)        SHARED_EXT=".dylib"; SHARED_LDFLAGS="-dynamiclib"   ;;
  *) echo "Unsupported platform: $(uname -s)"; exit 1 ;;
esac

# Helpers
slurp() { [[ -f "$1" ]] && cat "$1" || printf ""; }
sanitize_val() {
  # strip CR, ASCII quotes, smart quotes; squeeze spaces
  printf '%s' "$1" | tr -d '\r"' | sed -e "s/[“”‘’]//g" -e 's/[[:space:]]\+/ /g' -e 's/^ *//' -e 's/ *$//'
}

# Load & sanitize
SUPPORTED_ANALYZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/analyzer_flags.txt")")
SUPPORTED_CODE_GENERATION_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/code_generation_flags.txt")")
SUPPORTED_DEBUG_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/debug_flags.txt")")
SUPPORTED_INSTRUMENTATION_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/instrumentation_flags.txt")")
SUPPORTED_OPTIMIZATION_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/optimization_flags.txt")")
SUPPORTED_WARNING_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/warning_flags.txt")")

SUPPORTED_ADDRESS_SANITIZER_FLAGS=""
SUPPORTED_CFI_SANITIZER_FLAGS=""
SUPPORTED_DATAFLOW_SANITIZER_FLAGS=""
SUPPORTED_HWADDRESS_SANITIZER_FLAGS=""
SUPPORTED_LEAK_SANITIZER_FLAGS=""
SUPPORTED_MEMORY_SANITIZER_FLAGS=""
SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS=""
SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=""
SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS=""
SUPPORTED_THREAD_SANITIZER_FLAGS=""
SUPPORTED_UNDEFINED_SANITIZER_FLAGS=""

IFS=',' read -r -a SAN_ARR <<< "${sanitizers:-}"
for s in "${SAN_ARR[@]}"; do
  case "$s" in
    address)          SUPPORTED_ADDRESS_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/address_sanitizer_flags.txt")");;
    cfi)              SUPPORTED_CFI_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/cfi_sanitizer_flags.txt")");;
    dataflow)         SUPPORTED_DATAFLOW_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/dataflow_sanitizer_flags.txt")");;
    hwaddress)        SUPPORTED_HWADDRESS_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/hwaddress_sanitizer_flags.txt")");;
    leak)             SUPPORTED_LEAK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/leak_sanitizer_flags.txt")");;
    memory)           SUPPORTED_MEMORY_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/memory_sanitizer_flags.txt")");;
    pointer_overflow) SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/pointer_overflow_sanitizer_flags.txt")");;
    safe_stack)       SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/safe_stack_sanitizer_flags.txt")");;
    shadow_call_stack)SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/shadow_call_stack_sanitizer_flags.txt")");;
    thread)           SUPPORTED_THREAD_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/thread_sanitizer_flags.txt")");;
    undefined)        SUPPORTED_UNDEFINED_SANITIZER_FLAGS=$(sanitize_val "$(slurp "./.flags/${c_compiler}/undefined_sanitizer_flags.txt")");;
    "" ) ;;
    * ) echo "Warning: unknown sanitizer '$s' ignored." >&2 ;;
  esac
done

emit_header() {
  cat > Makefile <<EOF
.PHONY: all format lint analyze check clean cleanup

CC=$c_compiler
COMPILATION_FLAGS=-std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE -D_GNU_SOURCE -D__BSD_VISIBLE -Werror
SUPPORTED_ANALYZER_FLAGS=$SUPPORTED_ANALYZER_FLAGS
SUPPORTED_CODE_GENERATION_FLAGS=$SUPPORTED_CODE_GENERATION_FLAGS
SUPPORTED_DEBUG_FLAGS=$SUPPORTED_DEBUG_FLAGS
SUPPORTED_INSTRUMENTATION_FLAGS=$SUPPORTED_INSTRUMENTATION_FLAGS
SUPPORTED_OPTIMIZATION_FLAGS=$SUPPORTED_OPTIMIZATION_FLAGS
SUPPORTED_WARNING_FLAGS=$SUPPORTED_WARNING_FLAGS
SUPPORTED_ADDRESS_SANITIZER_FLAGS=$SUPPORTED_ADDRESS_SANITIZER_FLAGS
SUPPORTED_CFI_SANITIZER_FLAGS=$SUPPORTED_CFI_SANITIZER_FLAGS
SUPPORTED_DATAFLOW_SANITIZER_FLAGS=$SUPPORTED_DATAFLOW_SANITIZER_FLAGS
SUPPORTED_HWADDRESS_SANITIZER_FLAGS=$SUPPORTED_HWADDRESS_SANITIZER_FLAGS
SUPPORTED_LEAK_SANITIZER_FLAGS=$SUPPORTED_LEAK_SANITIZER_FLAGS
SUPPORTED_MEMORY_SANITIZER_FLAGS=$SUPPORTED_MEMORY_SANITIZER_FLAGS
SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS=$SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS
SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$SUPPORTED_SAFE_STACK_SANITIZER_FLAGS
SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS=$SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS
SUPPORTED_THREAD_SANITIZER_FLAGS=$SUPPORTED_THREAD_SANITIZER_FLAGS
SUPPORTED_UNDEFINED_SANITIZER_FLAGS=$SUPPORTED_UNDEFINED_SANITIZER_FLAGS
CLANG_TIDY_CHECKS=-checks=*,-llvmlibc-restrict-system-libc-headers,-altera-struct-pack-align,-readability-identifier-length,-altera-unroll-loops,-cppcoreguidelines-init-variables,-cert-err33-c,-modernize-macro-to-enum,-bugprone-easily-swappable-parameters,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-altera-id-dependent-backward-branch,-concurrency-mt-unsafe,-misc-unused-parameters,-hicpp-signed-bitwise,-google-readability-todo,-cert-msc30-c,-cert-msc50-cpp,-readability-function-cognitive-complexity,-clang-analyzer-security.insecureAPI.strcpy,-cert-env33-c,-android-cloexec-accept,-clang-analyzer-security.insecureAPI.rand,-misc-include-cleaner,-cppcoreguidelines-macro-to-enum
LIBRARIES=
PROGRAMS=
BINARIES=
LIBS=
SOURCES=

EOF
}

emit_lib_targets() {
  local base="$1" file="$2"
  cat >> Makefile <<EOF
lib${base}${SHARED_EXT}: ${file}
	@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \
	  \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_CODE_GENERATION_FLAGS) \
	  \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \
	  \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \
	  \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \
	  \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \
	  \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \
	  \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \
	  \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \
	  \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include $SHARED_LDFLAGS -o lib${base}${SHARED_EXT} ${file} \$(LIBRARIES)

LIBS += lib${base}${SHARED_EXT}

lib${base}-traceable${SHARED_EXT}: ${file}
	@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \
	  \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_CODE_GENERATION_FLAGS) \
	  \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \
	  \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \
	  \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \
	  \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \
	  \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \
	  \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \
	  \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \
	  \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include $SHARED_LDFLAGS -o lib${base}-traceable${SHARED_EXT} ${file} \$(LIBRARIES)

LIBS += lib${base}-traceable${SHARED_EXT}

EOF
}

emit_exe_targets() {
  local base="$1" file="$2"
  cat >> Makefile <<EOF
SOURCES += ${file}

${base}: ${file}
	@echo "Compiling ${file} -> ${base}"
	@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \
	  \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_CODE_GENERATION_FLAGS) \
	  \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \
	  \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \
	  \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \
	  \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \
	  \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \
	  \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \
	  \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \
	  \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o ${base} ${file} \$(LIBRARIES)

PROGRAMS += ${base}
BINARIES += ${base}

${base}-traceable: ${file}
	@echo "Compiling ${file} -> ${base}-traceable"
	@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \
	  \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_CODE_GENERATION_FLAGS) \
	  \$(SUPPORTED_DEBUG_FLAGS) \$(SUPPORTED_INSTRUMENTATION_FLAGS) \
	  \$(SUPPORTED_OPTIMIZATION_FLAGS) \$(SUPPORTED_WARNING_FLAGS) \
	  \$(SUPPORTED_ADDRESS_SANITIZER_FLAGS) \$(SUPPORTED_CFI_SANITIZER_FLAGS) \
	  \$(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) \$(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \
	  \$(SUPPORTED_LEAK_SANITIZER_FLAGS) \$(SUPPORTED_MEMORY_SANITIZER_FLAGS) \
	  \$(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) \$(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \
	  \$(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) \$(SUPPORTED_THREAD_SANITIZER_FLAGS) \
	  \$(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o ${base}-traceable ${file} \$(LIBRARIES)

PROGRAMS += ${base}-traceable
BINARIES += ${base}-traceable

EOF
}

emit_footer() {
  cat >> Makefile <<EOF
cleanup:
	@rm -rf *.dSYM || true
	@rm -f *.gcda *.gcno || true

format:
	@echo "Formatting source code..."
	@${clang_format_name} -i --style=file \$(SOURCES)

lint:
	@echo "Running clang-tidy for static code analysis..."
	@${clang_tidy_name} \$(SOURCES) -quiet --warnings-as-errors='*' \$(CLANG_TIDY_CHECKS) -- \$(COMPILATION_FLAGS) \$(CFLAGS) -I/usr/local/include

check:
	@echo "Running cppcheck for static code analysis..."
	@${cppcheck_name} -D__cppcheck__ --error-exitcode=1 --force --quiet --inline-suppr --check-level=exhaustive --library=posix --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression --suppress=checkersReport -I/usr/local/include \$(SOURCES)

clean:
	@rm -f \$(PROGRAMS) \$(LIBS)
	@rm -rf \$(BINARIES) *.dSYM || true
	@rm -f *.gcda *.gcno *.out *.gcov || true

EOF

  if [[ "$c_compiler" == *clang* ]]; then
    cat >> Makefile <<'EOF'
analyze:
	@echo "Running $(CC) static analyzer..."
	@$(CC) --analyze --analyzer-output text -Xclang -analyzer-checker=core -Xclang -analyzer-checker=deadcode -Xclang -analyzer-checker=security -Xclang -analyzer-disable-checker=security.insecureAPI.DeprecatedOrUnsafeBufferHandling -Xclang -analyzer-disable-checker=security.insecureAPI.rand -Xclang -analyzer-checker=unix -I/usr/local/include $(CFLAGS) $(COMPILATION_FLAGS) $(SOURCES)

EOF
    echo 'all: cleanup format $(PROGRAMS) $(LIBS) lint analyze check' >> Makefile
  else
    echo 'all: cleanup format $(PROGRAMS) $(LIBS) lint check' >> Makefile
  fi

  echo "" >> Makefile
  echo "# Makefile generated in $(pwd)" >> Makefile
}

generate_makefile() {
  emit_header
  shopt -s nullglob
  for file in *.c; do
    [[ -f "$file" ]] || continue
    base="${file%.c}"
    if [[ "$base" == "testlib-1" || "$base" == "testlib-2" ]]; then
      emit_lib_targets "$base" "$file"
    else
      emit_exe_targets "$base" "$file"
    fi
  done
  shopt -u nullglob
  emit_footer

  # Safety: if *any* literal "\n" slipped in, convert them to real newlines.
  if grep -q '\\n' Makefile; then
    # Portable in-place edit: write a temp, then move.
    awk '{gsub(/\\n/,"\n")}1' Makefile > Makefile.__tmp && mv Makefile.__tmp Makefile
  fi
}

process_directories() {
  local dir="$1"
  shopt -s nullglob
  for subdir in "$dir"/*/; do
    [[ -d "$subdir" ]] || continue
    case "$(basename "$subdir")" in
      .|..|.git|.github|.gitlab|.flags|flags|.vscode|.idea|build|cmake-build-*|dist|target|out|bin|obj|.*) continue ;;
    esac
    echo "Processing directory: $subdir"
    if ! git ls-files --error-unmatch "$subdir" >/dev/null 2>&1; then
      rm -rf "$subdir"; echo "Deleted '$subdir'"
    else
      if find "$subdir" -maxdepth 1 -name "*.c" -print -quit | grep -q .; then
        ( cd "$subdir" && generate_makefile )
      fi
    fi
    process_directories "$subdir"
  done
  shopt -u nullglob
}

echo "c_compiler is set to: $c_compiler"
[[ -d "./.flags" ]] || ./generate-flags.sh
process_directories "."
[[ -x ./cleanup.sh ]] && ./cleanup.sh || true

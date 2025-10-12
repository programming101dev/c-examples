#!/usr/bin/env bash
# generate-makefiles.sh — create Makefiles only in subdirs that have C sources
# Platforms: macOS (BSD make), FreeBSD, Linux

set -euo pipefail

# ----------------- args & usage -----------------
c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"
sanitizers=""

usage() {
  cat <<EOF
Usage: $0 -c <C compiler> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>] [-s <sanitizers>]
  -c c compiler     gcc, clang, clang-20, or absolute path (selects .flags/<name>)
  -f clang-format   clang-format binary name (default: clang-format)
  -t clang-tidy     clang-tidy binary name   (default: clang-tidy)
  -k cppcheck       cppcheck binary name     (default: cppcheck)
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
[[ -d "./.flags"     ]] || { echo "Error: ./.flags is missing; expected precomputed flags." >&2; exit 1; }

compiler_key="${c_compiler##*/}"
flags_dir="./.flags/${compiler_key}"
[[ -d "$flags_dir" ]] || echo "Warning: $flags_dir does not exist, continuing with empty flags." >&2

# ----------------- platform specifics -----------------
SHARED_EXT=""; SHARED_LDFLAGS=""
case "$(uname -s)" in
  Linux|FreeBSD) SHARED_EXT=".so";    SHARED_LDFLAGS="-shared -fPIC" ;;
  Darwin)        SHARED_EXT=".dylib"; SHARED_LDFLAGS="-dynamiclib"   ;;
  *) echo "Unsupported platform: $(uname -s)"; exit 1 ;;
esac

# ----------------- helpers -----------------
slurp() { [[ -f "$1" ]] && cat "$1" || printf ""; }
sanitize_val() {
  printf '%s' "$1" | tr -d '\r"' | sed -e 's/[“”‘’]//g' -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//' -e 's/[[:space:]][[:space:]]*/ /g'
}

# ----------------- load flags -----------------
SUPPORTED_ANALYZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/analyzer_flags.txt")")
SUPPORTED_CODE_GENERATION_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/code_generation_flags.txt")")
SUPPORTED_DEBUG_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/debug_flags.txt")")
SUPPORTED_INSTRUMENTATION_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/instrumentation_flags.txt")")
SUPPORTED_OPTIMIZATION_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/optimization_flags.txt")")
SUPPORTED_WARNING_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/warning_flags.txt")")

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

# ----------------- sanitizer selection -----------------
declare -a SAN_ARR=()
if [[ -n "${sanitizers-}" ]]; then IFS=',' read -r -a SAN_ARR <<< "$sanitizers"; fi
if ((${#SAN_ARR[@]} > 0)); then
  for s in "${SAN_ARR[@]}"; do
    case "$s" in
      address)           SUPPORTED_ADDRESS_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/address_sanitizer_flags.txt")");;
      cfi)               SUPPORTED_CFI_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/cfi_sanitizer_flags.txt")");;
      dataflow)          SUPPORTED_DATAFLOW_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/dataflow_sanitizer_flags.txt")");;
      hwaddress)         SUPPORTED_HWADDRESS_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/hwaddress_sanitizer_flags.txt")");;
      leak)              SUPPORTED_LEAK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/leak_sanitizer_flags.txt")");;
      memory)            SUPPORTED_MEMORY_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/memory_sanitizer_flags.txt")");;
      pointer_overflow)  SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/pointer_overflow_sanitizer_flags.txt")");;
      safe_stack)        SUPPORTED_SAFE_STACK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/safe_stack_sanitizer_flags.txt")");;
      shadow_call_stack) SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/shadow_call_stack_sanitizer_flags.txt")");;
      thread)            SUPPORTED_THREAD_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/thread_sanitizer_flags.txt")");;
      undefined)         SUPPORTED_UNDEFINED_SANITIZER_FLAGS=$(sanitize_val "$(slurp "${flags_dir}/undefined_sanitizer_flags.txt")");;
      "" ) ;;
      * ) echo "Warning: unknown sanitizer '$s' ignored." >&2 ;;
    esac
  done
fi

# ----------------- Makefile emitters -----------------
emit_header() {
  # Use only portable make features. BSD make supports .SILENT: and ?=.
  cat > Makefile <<'EOF'
.PHONY: all format lint analyze check clean cleanup
.SILENT:
# Prefix every command with $(Q). See below for the default.
Q?=@

EOF

  cat >> Makefile <<EOF
CC=$c_compiler
CLANG_FORMAT=$clang_format_name
CLANG_TIDY=$clang_tidy_name
CPPCHECK=$cppcheck_name

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

# write a TAB-prefixed line
t() { printf '\t%s\n' "$1" >> Makefile; }

emit_lib_targets() {
  local base="$1" file="$2"
  {
    printf 'lib%s%s: %s\n' "$base" "$SHARED_EXT" "$file"
    t '$(Q)echo "[link] lib'"$base$SHARED_EXT"'"'
    t '$(Q)$(CC) $(COMPILATION_FLAGS) $(CFLAGS) \'
    t '  $(SUPPORTED_ANALYZER_FLAGS) $(SUPPORTED_CODE_GENERATION_FLAGS) \'
    t '  $(SUPPORTED_DEBUG_FLAGS) $(SUPPORTED_INSTRUMENTATION_FLAGS) \'
    t '  $(SUPPORTED_OPTIMIZATION_FLAGS) $(SUPPORTED_WARNING_FLAGS) \'
    t '  $(SUPPORTED_ADDRESS_SANITIZER_FLAGS) $(SUPPORTED_CFI_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) $(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_LEAK_SANITIZER_FLAGS) $(SUPPORTED_MEMORY_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) $(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) $(SUPPORTED_THREAD_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include '"$SHARED_LDFLAGS"' -o lib'"$base$SHARED_EXT"' '"$file"' $(LIBRARIES)'
    printf '\nLIBS += lib%s%s\n\n' "$base" "$SHARED_EXT"

    printf 'lib%s-traceable%s: %s\n' "$base" "$SHARED_EXT" "$file"
    t '$(Q)echo "[link] lib'"$base"'-traceable'"$SHARED_EXT"'"'
    t '$(Q)$(CC) $(COMPILATION_FLAGS) $(CFLAGS) \'
    t '  $(SUPPORTED_ANALYZER_FLAGS) $(SUPPORTED_CODE_GENERATION_FLAGS) \'
    t '  $(SUPPORTED_DEBUG_FLAGS) $(SUPPORTED_INSTRUMENTATION_FLAGS) \'
    t '  $(SUPPORTED_OPTIMIZATION_FLAGS) $(SUPPORTED_WARNING_FLAGS) \'
    t '  $(SUPPORTED_ADDRESS_SANITIZER_FLAGS) $(SUPPORTED_CFI_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) $(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_LEAK_SANITIZER_FLAGS) $(SUPPORTED_MEMORY_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) $(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) $(SUPPORTED_THREAD_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include '"$SHARED_LDFLAGS"' -o lib'"$base"'-traceable'"$SHARED_EXT"' '"$file"' $(LIBRARIES)'
    printf '\nLIBS += lib%s-traceable%s\n\n' "$base" "$SHARED_EXT"
  } >> Makefile
}

emit_exe_targets() {
  local base="$1" file="$2"
  {
    printf 'SOURCES += %s\n\n' "$file"

    printf '%s: %s\n' "$base" "$file"
    t '$(Q)echo "[cc] '"$file"' -> '"$base"'"'
    t '$(Q)$(CC) $(COMPILATION_FLAGS) $(CFLAGS) \'
    t '  $(SUPPORTED_ANALYZER_FLAGS) $(SUPPORTED_CODE_GENERATION_FLAGS) \'
    t '  $(SUPPORTED_DEBUG_FLAGS) $(SUPPORTED_INSTRUMENTATION_FLAGS) \'
    t '  $(SUPPORTED_OPTIMIZATION_FLAGS) $(SUPPORTED_WARNING_FLAGS) \'
    t '  $(SUPPORTED_ADDRESS_SANITIZER_FLAGS) $(SUPPORTED_CFI_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) $(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_LEAK_SANITIZER_FLAGS) $(SUPPORTED_MEMORY_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) $(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) $(SUPPORTED_THREAD_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o '"$base"' '"$file"' $(LIBRARIES)'
    printf '\nPROGRAMS += %s\nBINARIES += %s\n\n' "$base" "$base"

    printf '%s-traceable: %s\n' "$base" "$file"
    t '$(Q)echo "[cc] '"$file"' -> '"$base"'-traceable"'
    t '$(Q)$(CC) $(COMPILATION_FLAGS) $(CFLAGS) \'
    t '  $(SUPPORTED_ANALYZER_FLAGS) $(SUPPORTED_CODE_GENERATION_FLAGS) \'
    t '  $(SUPPORTED_DEBUG_FLAGS) $(SUPPORTED_INSTRUMENTATION_FLAGS) \'
    t '  $(SUPPORTED_OPTIMIZATION_FLAGS) $(SUPPORTED_WARNING_FLAGS) \'
    t '  $(SUPPORTED_ADDRESS_SANITIZER_FLAGS) $(SUPPORTED_CFI_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_DATAFLOW_SANITIZER_FLAGS) $(SUPPORTED_HWADDRESS_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_LEAK_SANITIZER_FLAGS) $(SUPPORTED_MEMORY_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_POINTER_OVERFLOW_SANITIZER_FLAGS) $(SUPPORTED_SAFE_STACK_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_SHADOW_CALL_STACK_SANITIZER_FLAGS) $(SUPPORTED_THREAD_SANITIZER_FLAGS) \'
    t '  $(SUPPORTED_UNDEFINED_SANITIZER_FLAGS) -I/usr/local/include -o '"$base"'-traceable '"$file"' $(LIBRARIES)'
    printf '\nPROGRAMS += %s-traceable\nBINARIES += %s-traceable\n\n' "$base" "$base"
  } >> Makefile
}

emit_footer() {
  {
    printf 'cleanup:\n'
    t '$(Q)rm -rf *.dSYM || true'
    t '$(Q)rm -f *.gcda *.gcno || true'
    printf '\n'

    printf 'format:\n'
    t '$(Q)echo "[format] $(CLANG_FORMAT)"'
    t '$(Q)set -- $(SOURCES); if [ "$$#" -gt 0 ]; then $(CLANG_FORMAT) -i --style=file "$$@"; else echo "[format] no sources"; fi'
    printf '\n\n'

    printf 'lint:\n'
    t '$(Q)echo "[clang-tidy]"'
    t '$(Q)set -- $(SOURCES); if [ "$$#" -gt 0 ]; then $(CLANG_TIDY) "$$@" -quiet --warnings-as-errors="*" $(CLANG_TIDY_CHECKS) -- $(COMPILATION_FLAGS) $(CFLAGS) -I/usr/local/include; else echo "[clang-tidy] no sources"; fi'
    printf '\n\n'

    printf 'check:\n'
    t '$(Q)echo "[cppcheck]"'
    t '$(Q)set -- $(SOURCES); if [ "$$#" -gt 0 ]; then $(CPPCHECK) -D__cppcheck__ --error-exitcode=1 --force --quiet --inline-suppr --check-level=exhaustive --library=posix --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression --suppress=checkersReport -I/usr/local/include "$$@"; else echo "[cppcheck] no sources"; fi'
    printf '\n\n'

    printf 'clean:\n'
    t '$(Q)rm -f $(PROGRAMS) $(LIBS)'
    t '$(Q)rm -rf $(BINARIES) *.dSYM || true'
    t '$(Q)rm -f *.gcda *.gcno *.out *.gcov || true'
    printf '\n'
  } >> Makefile

  if [[ "$c_compiler" == *clang* ]]; then
    {
      printf 'analyze:\n'
      t '$(Q)echo "[clang-sa]"'
      t '$(Q)set -- $(SOURCES); if [ "$$#" -gt 0 ]; then $(CC) --analyze --analyzer-output text -Xclang -analyzer-checker=core -Xclang -analyzer-checker=deadcode -Xclang -analyzer-checker=security -Xclang -analyzer-disable-checker=security.insecureAPI.DeprecatedOrUnsafeBufferHandling -Xclang -analyzer-disable-checker=security.insecureAPI.rand -Xclang -analyzer-checker=unix -I/usr/local/include $(CFLAGS) $(COMPILATION_FLAGS) "$$@"; else echo "[clang-sa] no sources"; fi'
      printf '\n'
      printf 'all: cleanup format $(PROGRAMS) $(LIBS) lint analyze check\n'
    } >> Makefile
  else
    echo 'all: cleanup format $(PROGRAMS) $(LIBS) lint check' >> Makefile
  fi

  printf '\n# Makefile generated in %s\n' "$(pwd)" >> Makefile
}

# ----------------- generation logic -----------------
generate_makefile() {
  # Only create a Makefile in directories that actually have sources.
  shopt -s nullglob
  set -- *.c
  if [ "$#" -eq 0 ]; then
    shopt -u nullglob
    return 0
  fi

  emit_header
  for file in "$@"; do
    base="${file%.c}"
    if [[ "$base" == "testlib-1" || "$base" == "testlib-2" ]]; then
      emit_lib_targets "$base" "$file"
    else
      emit_exe_targets "$base" "$file"
    fi
  done
  shopt -u nullglob
  emit_footer
}

process_directories() {
  local dir="$1"
  shopt -s nullglob
  for sub in "$dir"/*/; do
    [[ -d "$sub" ]] || continue
    case "$(basename "$sub")" in
      .|..|.git|.github|.gitlab|.flags|flags|.vscode|.idea|build|cmake-build-*|dist|target|out|bin|obj|.*) continue ;;
    esac

    # normalize to avoid double slashes in prints
    local subdir="${sub%/}"
    echo "Processing directory: $subdir"

    # Remove a stale Makefile if the folder has no C sources
    if [[ -f "$subdir/Makefile" ]]; then
      shopt -s nullglob; set -- "$subdir"/*.c; n="$#"; shopt -u nullglob
      if [ "$n" -eq 0 ]; then rm -f "$subdir/Makefile"; fi
    fi

    # Skip untracked directories entirely
    if ! git ls-files --error-unmatch "$subdir" >/dev/null 2>&1; then
      rm -rf "$subdir"; echo "Deleted untracked dir '$subdir'"
    else
      # Generate only if there are sources
      shopt -s nullglob; set -- "$subdir"/*.c; n="$#"; shopt -u nullglob
      if [ "$n" -gt 0 ]; then
        ( cd "$subdir" && generate_makefile )
      fi
    fi

    process_directories "$subdir"
  done
  shopt -u nullglob
}

echo "Using compiler key: ${compiler_key} (from -c '${c_compiler}')"
process_directories "."
[[ -x ./cleanup.sh ]] && ./cleanup.sh || true

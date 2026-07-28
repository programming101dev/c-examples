#!/usr/bin/env bash
set -euo pipefail

# --- opt-in coverage / profiling (P101) ---------------------------------
# Pull the long flags out before the normal option parser and export them.
# The shared CMakeLists reads P101_COVERAGE / P101_PROFILE at configure time
# and instruments the compile + link. Absent => nothing changes. If a parent
# (e.g. update.sh / build-all.sh) already exported them, they are inherited.
_p101_argv=()
for _p101_a in "$@"; do
  case "$_p101_a" in
    --coverage) export P101_COVERAGE=1 ;;
    --profile)  export P101_PROFILE=1 ;;
    *)          _p101_argv+=("$_p101_a") ;;
  esac
done
if ((${#_p101_argv[@]})); then set -- "${_p101_argv[@]}"; else set --; fi
unset _p101_argv _p101_a
# QoL: a bare first argument is taken as the C compiler, i.e.
#   ./change-compiler.sh gcc-16   ==   ./change-compiler.sh -c gcc-16
if [[ "${1-}" != "" && "${1-}" != -* ]]; then set -- -c "$@"; fi
# ------------------------------------------------------------------------

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"
sanitizers=""
sanitizers_passed=false

usage() {
  echo "Usage: $0 -c <c compiler> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>] [-s <sanitizers>]" >&2
  echo "  -c c compiler    Specify the C compiler name, for example gcc or clang" >&2
  echo "  -f clang-format  Specify the clang-format name, for example clang-format-17" >&2
  echo "  -t clang-tidy    Specify the clang-tidy name, for example clang-tidy-17" >&2
  echo "  -k cppcheck      Specify the cppcheck name, for example cppcheck" >&2
  echo "  -s sanitizers    Comma list, for example address,undefined. If omitted, uses sanitizers.txt" >&2
  exit 1
}

# --help / -h -> usage, exit 0 (P101 uniform CLI help)
case " $* " in *" --help "*|*" -h "*) ( usage ) || true; exit 0 ;; esac

while getopts ":c:f:t:k:s:" opt; do
  case $opt in
    c) c_compiler="$OPTARG" ;;
    f) clang_format_name="$OPTARG" ;;
    t) clang_tidy_name="$OPTARG" ;;
    k) cppcheck_name="$OPTARG" ;;
    s) sanitizers="$OPTARG"; sanitizers_passed=true ;;
    \?|:) usage ;;
  esac
done

if [ -z "$c_compiler" ]; then
  echo "Error: C compiler argument (-c) is required." >&2
  usage
fi

resolve_tool() {
  local tool="$1"
  if [[ "$tool" = /* ]]; then
    [[ -x "$tool" ]] || { echo "Error: '$tool' not executable" >&2; exit 2; }
    printf '%s\n' "$tool"
    return 0
  fi

  local found
  found="$(command -v "$tool" 2>/dev/null || true)"
  [[ -n "$found" ]] || { echo "Error: '$tool' not found in PATH" >&2; exit 2; }
  printf '%s\n' "$found"
}

CC_PATH="$(resolve_tool "$c_compiler")"
CLANG_FORMAT_PATH="$(resolve_tool "$clang_format_name")"
CLANG_TIDY_PATH="$(resolve_tool "$clang_tidy_name")"
CPPCHECK_PATH="$(resolve_tool "$cppcheck_name")"

if ! $sanitizers_passed; then
  if [ -f "sanitizers.txt" ]; then
    # Strip all whitespace and ignore comments after '#'
    sanitizers="$(sed 's/#.*$//g' sanitizers.txt | tr -d '[:space:]')"
    echo "Sanitizers loaded from sanitizers.txt: ${sanitizers:-<none>}"
  else
    echo "Warning: sanitizers.txt not found and no -s provided. Using no sanitizers."
    sanitizers=""
  fi
else
  echo "Sanitizers specified via command-line: ${sanitizers:-<none>}"
fi

# Clean build dir to avoid stale cache
rm -rf build
mkdir -p build

echo "Configuring with:"
echo "  CC               = $CC_PATH"
echo "  clang-format     = $CLANG_FORMAT_PATH"
echo "  clang-tidy       = $CLANG_TIDY_PATH"
echo "  cppcheck         = $CPPCHECK_PATH"
echo "  sanitizers       = ${sanitizers:-<none>}"

cmake -S . -B build \
  -DCMAKE_C_COMPILER="$CC_PATH" \
  -DCLANG_FORMAT_NAME="$CLANG_FORMAT_PATH" \
  -DCLANG_TIDY_NAME="$CLANG_TIDY_PATH" \
  -DCPPCHECK_NAME="$CPPCHECK_PATH" \
  -DSANITIZER_LIST="$sanitizers" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

#!/usr/bin/env bash
# check-env (C-only) — verify required tools exist and the C compiler works

set -euo pipefail

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"
do_compile_test=true   # set false via --no-compile-test

usage() {
  cat <<EOF
Usage: $0 -c <C compiler> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>] [--no-compile-test]
  -c <cc>        C compiler (e.g. gcc, clang, gcc-15, /opt/homebrew/bin/gcc-15)
  -f <name>      clang-format executable (default: clang-format)
  -t <name>      clang-tidy   executable (default: clang-tidy)
  -k <name>      cppcheck     executable (default: cppcheck)
  --no-compile-test  Skip compiling a trivial C program (presence check only)

Exit status: number of missing/broken tools (0 = all good).
EOF
}

# --- parse args (support a long flag too) ---
args=()
for a in "$@"; do
  if [[ "$a" == "--no-compile-test" ]]; then
    do_compile_test=false
  else
    args+=("$a")
  fi
done
# Guard against empty array under set -u on older Bash
if ((${#args[@]} > 0)); then
  set -- "${args[@]}"
else
  set --
fi

while getopts ":c:f:t:k:h" opt; do
  case "$opt" in
    c) c_compiler="$OPTARG" ;;
    f) clang_format_name="$OPTARG" ;;
    t) clang_tidy_name="$OPTARG" ;;
    k) cppcheck_name="$OPTARG" ;;
    h|\?|:) usage; exit 2 ;;
  esac
done

# --- required arg ---
if [[ -z "$c_compiler" ]]; then
  echo "Error: -c <C compiler> is required." >&2
  usage; exit 2
fi

# --- helpers ---
resolve_exe() {
  # resolve_exe <name-or-absolute-path>
  local val="$1"
  local path=""
  if [[ "$val" = /* ]]; then
    path="$val"
    [[ -x "$path" ]] || { echo "missing (not executable): $val"; return 1; }
  else
    if ! path="$(command -v "$val" 2>/dev/null)"; then
      echo "missing: $val"
      return 1
    fi
  fi
  echo "$path"
  return 0
}

compile_test() {
  # compile_test <cc>
  local cc="$1"
  local tmpdir src exe
  tmpdir="$(mktemp -d 2>/dev/null || mktemp -d -t ccprobe)"
  src="$tmpdir/t.c"
  exe="$tmpdir/a.out"
  printf 'int main(void){return 0;}\n' >"$src"
  if "$cc" -x c "$src" -o "$exe" >/dev/null 2>&1; then
    rm -rf "$tmpdir"
    return 0
  else
    rm -rf "$tmpdir"
    return 1
  fi
}

# --- check tools ---
missing=0

CC_PATH="$(resolve_exe "$c_compiler")" || ((missing++))
CF_PATH="$(resolve_exe "$clang_format_name")" || ((missing++))
CT_PATH="$(resolve_exe "$clang_tidy_name")"  || ((missing++))
CP_PATH="$(resolve_exe "$cppcheck_name")"    || ((missing++))

# Extra hint on macOS when user points to Apple’s gcc stub
if [[ "$(uname -s)" == "Darwin" && -n "${CC_PATH:-}" && "$CC_PATH" == "/usr/bin/gcc" ]]; then
  echo "note: '/usr/bin/gcc' on macOS is AppleClang. Prefer 'clang' or Homebrew gcc-(N)." >&2
fi

# Optional: verify the C compiler can actually produce an executable
if $do_compile_test && [[ -n "${CC_PATH:-}" ]]; then
  if ! compile_test "$CC_PATH"; then
    echo "broken: $c_compiler (cannot compile a trivial C program)"
    ((missing++))
  fi
fi

echo "Total missing/broken tools: $missing"
exit "$missing"

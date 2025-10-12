#!/usr/bin/env bash
# run-make-tree.sh — run make in each directory that has a Makefile
# Portable: macOS (BSD make), FreeBSD, Linux

set -euo pipefail

start_dir="."
target="all"
clean_first=false
jobs=""
keep_going=false
quiet=false

usage() {
  cat <<'USAGE'
Usage: run-make-tree.sh [-d <dir>] [-t <target>] [-c] [-j <N>] [-k] [-q] [-h]
  -d <dir>   Start directory (default: .)
  -t <tgt>   make target (default: all)
  -c         clean first (run "make clean" before target)
  -j <N>     parallel jobs, passed to make as -jN
  -k         keep going (do not stop on first failure; report summary at end)
  -q         quiet (less chatter)
  -h         help

Notes:
- Skips *.dSYM directories.
- Detects Makefile/makefile/GNUmakefile.
USAGE
  exit 1
}

while getopts ":d:t:j:ckqh" opt; do
  case "$opt" in
    d) start_dir="$OPTARG" ;;
    t) target="$OPTARG" ;;
    j) jobs="-j${OPTARG}" ;;
    c) clean_first=true ;;
    k) keep_going=true ;;
    q) quiet=true ;;
    h|*) usage ;;
  esac
done

[[ -d "$start_dir" ]] || { echo "Error: start directory '$start_dir' does not exist." >&2; exit 1; }

# Collect dirs, pruning common noise. BSD/GNU find compatible.
all_dirs=()
while IFS= read -r line; do
  [[ -z "$line" ]] && continue
  all_dirs+=("$line")
done < <(
  find "$start_dir" \
    -path "*/.git"            -prune -o \
    -path "*/.github"         -prune -o \
    -path "*/.gitlab"         -prune -o \
    -path "*/node_modules"    -prune -o \
    -path "*/build"           -prune -o \
    -path "*/cmake-build-*"   -prune -o \
    -path "*/out"             -prune -o \
    -path "*/dist"            -prune -o \
    -path "*/target"          -prune -o \
    -path "*/bin"             -prune -o \
    -path "*/obj"             -prune -o \
    -path "*/.dSYM"           -prune -o \
    -type d -print | LC_ALL=C sort
)

has_makefile() {
  local d="$1"
  [[ -f "$d/Makefile" || -f "$d/makefile" || -f "$d/GNUmakefile" ]]
}

is_dsym_dir() {
  local d="$1"
  [[ "$(basename "$d")" == *.dSYM ]]
}

run_make_in_dir() {
  local d="$1"
  local rc=0

  "$quiet" || printf "\n==> %s\n" "$d"

  if "$clean_first"; then
    "$quiet" || echo "  make clean"
    if ! make -C "$d" clean >/dev/null 2>&1; then
      make -C "$d" clean || return $?
    fi
  fi

  "$quiet" || echo "  make ${target} ${jobs}"
  # shellcheck disable=SC2086
  make -C "$d" ${jobs} "${target}" && rc=0 || rc=$?

  return "$rc"
}

fail_list=()
for dir in "${all_dirs[@]}"; do
  is_dsym_dir "$dir" && { "$quiet" || echo "Skipping dSYM dir: $dir"; continue; }
  has_makefile "$dir" || continue

  if ! run_make_in_dir "$dir"; then
    if "$keep_going"; then
      echo "!! make failed in: $dir"
      fail_list+=("$dir")
    else
      echo "Error: make failed in: $dir"
      exit 1
    fi
  fi
done

if ((${#fail_list[@]})); then
  echo
  echo "Build completed with failures in ${#fail_list[@]} dir(s):"
  for f in "${fail_list[@]}"; do
    echo "  - $f"
  done
  exit 1
else
  "$quiet" || printf "\nAll eligible directories built successfully.\n"
fi

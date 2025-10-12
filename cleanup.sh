#!/usr/bin/env bash
# run-make-tree.sh — walk a tree and run `make <target>` wherever a Makefile exists

set -euo pipefail

# -------- defaults --------
START_DIR="."
TARGET="clean"
JOBS=""
QUIET=0
DRYRUN=0
KEEPGO=0

# Built-in excludes (match either full path or basename)
EXCLUDES=(
  "*/.git/*" "*/.github/*" "*/.gitlab/*"
  "*.dSYM" "*/.dSYM/*"
  "*/build/*" "*/cmake-build-*/*" "*/out/*" "*/dist/*" "*/target/*" "*/bin/*" "*/obj/*"
  "*/.vscode/*" "*/.idea/*" "*/.venv/*" "*/node_modules/*"
  "*/.flags/*" "*/flags/*"
)

usage() {
  cat <<EOF
Usage: $0 [-d <start_dir>] [-t <target>] [-j <N>] [-q] [-n] [-k] [-x <glob>]...
  -d <dir>   Start directory (default: .)
  -t <name>  Make target to invoke (default: clean)
  -j <N>     Pass -jN to make
  -q         Quiet (suppress make stdout; keep errors)
  -n         Dry run (show what would run)
  -k         Keep going on errors (don't exit on first failure)
  -x <glob>  Additional exclude glob (repeatable); matched against full path
Examples:
  $0 -t all
  $0 -d src -t clean -j 8 -q
  $0 -x '*/third_party/*' -x '*/_build/*'
EOF
  exit 1
}

# -------- parse args --------
while getopts ":d:t:j:x:qnk" opt; do
  case "$opt" in
    d) START_DIR="$OPTARG" ;;
    t) TARGET="$OPTARG" ;;
    j) JOBS="$OPTARG" ;;
    x) EXCLUDES+=("$OPTARG") ;;
    q) QUIET=1 ;;
    n) DRYRUN=1 ;;
    k) KEEPGO=1 ;;
    *) usage ;;
  esac
done

# -------- helpers --------
should_skip() {
  # $1 = dir path
  local d="$1"
  case "$d" in
    */.git|*/.git/*|*/.github|*/.github/*|*/.gitlab|*/.gitlab/*) return 0 ;;
    *.dSYM|*/.dSYM/*) return 0 ;;
    */build|*/build/*|*/cmake-build-*|*/cmake-build-*/*|*/out|*/out/*|*/dist|*/dist/*|*/target|*/target/*|*/bin|*/bin/*|*/obj|*/obj/*) return 0 ;;
    */.vscode|*/.vscode/*|*/.idea|*/.idea/*|*/.venv|*/.venv/*|*/node_modules|*/node_modules/*) return 0 ;;
    */.flags|*/.flags/*|*/flags|*/flags/*) return 0 ;;
  esac
  for pat in "${EXCLUDES[@]}"; do
    [[ "$d" == $pat ]] && return 0
  done
  return 1
}

run_make_in() {
  # $1 = dir path
  local d="$1"
  [[ -f "$d/Makefile" ]] || return 0

  # Build status line without expanding an empty array
  if [[ -n "$JOBS" ]]; then
    echo "==> $d : make -j$JOBS $TARGET"
  else
    echo "==> $d : make $TARGET"
  fi

  if (( DRYRUN )); then
    return 0
  fi

  if (( QUIET )); then
    if [[ -n "$JOBS" ]]; then
      if ! make -C "$d" -j"$JOBS" "$TARGET" >/dev/null; then
        echo "Error: make failed in $d (target '$TARGET')"
        (( KEEPGO )) || exit 1
        return 1
      fi
    else
      if ! make -C "$d" "$TARGET" >/dev/null; then
        echo "Error: make failed in $d (target '$TARGET')"
        (( KEEPGO )) || exit 1
        return 1
      fi
    fi
  else
    if [[ -n "$JOBS" ]]; then
      if ! make -C "$d" -j"$JOBS" "$TARGET"; then
        echo "Error: make failed in $d (target '$TARGET')"
        (( KEEPGO )) || exit 1
        return 1
      fi
    else
      if ! make -C "$d" "$TARGET"; then
        echo "Error: make failed in $d (target '$TARGET')"
        (( KEEPGO )) || exit 1
        return 1
      fi
    fi
  fi
}

# -------- preflight --------
[[ -d "$START_DIR" ]] || { echo "Error: start directory not found: $START_DIR" >&2; exit 2; }

# -------- collect & sort dirs (portable, no mapfile) --------
ALL_DIRS=()
while IFS= read -r line; do
  ALL_DIRS+=("$line")
done < <(find "$START_DIR" -type d -print | LC_ALL=C sort)

# -------- walk --------
for dir in "${ALL_DIRS[@]}"; do
  should_skip "$dir" && continue
  run_make_in "$dir"
done

exit 0

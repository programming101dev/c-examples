#!/usr/bin/env bash
# Discover C compilers from PATH and prove each candidate can compile and link.
set -euo pipefail
CDPATH='' cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

case " $* " in
  *" --help "*|*" -h "*)
    printf '%s\n' "check-compilers.sh — discover usable C compilers from PATH."
    exit 0 ;;
esac
[[ "$#" -eq 0 ]] || { echo "Usage: ./check-compilers.sh" >&2; exit 2; }

output="supported_c_compilers.txt"
if [[ -L "$output" ]]; then
  [[ -s "$output" ]] || { echo "Error: shared compiler list is empty or dangling: $output" >&2; exit 1; }
  echo "$output is shared; leaving it unchanged."
  exit 0
fi

tmp_dir="$(mktemp -d "${TMPDIR:-/tmp}/p101-check-cc.XXXXXX")"
trap 'rm -rf -- "$tmp_dir"' EXIT
printf '%s\n' 'int main(void) { return 0; }' > "$tmp_dir/probe.c"

candidates=()
append_candidate() {
  local candidate="$1" existing
  [[ -x "$candidate" ]] || return 0
  candidate="$(basename "$candidate")"
  for existing in "${candidates[@]:-}"; do
    [[ "$existing" == "$candidate" ]] && return 0
  done
  candidates+=("$candidate")
}

old_ifs="$IFS"
IFS=:
for path_dir in $PATH; do
  [[ -n "$path_dir" ]] || path_dir=.
  shopt -s nullglob
  for candidate in \
    "$path_dir"/gcc "$path_dir"/gcc-[0-9]* "$path_dir"/gcc[0-9]* \
    "$path_dir"/clang "$path_dir"/clang-[0-9]* "$path_dir"/clang[0-9]* \
    "$path_dir"/clang-devel; do
    append_candidate "$candidate"
  done
  shopt -u nullglob
done
IFS="$old_ifs"

usable=()
for candidate in "${candidates[@]:-}"; do
  if "$candidate" "$tmp_dir/probe.c" -o "$tmp_dir/probe" >/dev/null 2>&1; then
    usable+=("$candidate")
  fi
done

[[ ${#usable[@]} -gt 0 ]] || {
  echo "Error: no C compiler on PATH could compile and link a probe." >&2
  exit 1
}
printf '%s\n' "${usable[@]}" > "$output"
printf 'Supported C compilers written to %s:\n' "$output"
printf '  %s\n' "${usable[@]}"

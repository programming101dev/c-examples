#!/usr/bin/env bash
# change-compiler.sh — choose tools for the Makefile-based c-examples tree.

set -euo pipefail
CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

c_compiler=""
clang_format_name="clang-format"
clang_tidy_name="clang-tidy"
cppcheck_name="cppcheck"

usage() {
  cat <<'USAGE' >&2
Usage: ./change-compiler.sh -c <cc> [-f <clang-format>] [-t <clang-tidy>] [-k <cppcheck>]

This repository is intentionally Makefile-based, not CMake-based. The command
updates the default tool variables in every example Makefile; you can still
override them at build time with make variables such as `make CC=clang`.
USAGE
  exit 1
}

case " $* " in *" --help "*|*" -h "*) usage; exit 0 ;; esac

if [[ "${1-}" != "" && "${1-}" != -* ]]; then
  set -- -c "$@"
fi

while getopts ":c:f:t:k:h" opt; do
  case "$opt" in
    c) c_compiler="$OPTARG" ;;
    f) clang_format_name="$OPTARG" ;;
    t) clang_tidy_name="$OPTARG" ;;
    k) cppcheck_name="$OPTARG" ;;
    h) usage ;;
    \?|:) usage ;;
  esac
done

[[ -n "$c_compiler" ]] || { echo "Error: -c <cc> is required." >&2; usage; }

resolve_tool() {
  local tool="$1"
  if [[ "$tool" = /* ]]; then
    [[ -x "$tool" ]] || { echo "Error: '$tool' not executable" >&2; exit 2; }
    printf '%s\n' "$tool"
    return 0
  fi
  command -v "$tool" >/dev/null 2>&1 || { echo "Error: '$tool' not found in PATH" >&2; exit 2; }
  printf '%s\n' "$tool"
}

cc_value="$(resolve_tool "$c_compiler")"
format_value="$(resolve_tool "$clang_format_name")"
tidy_value="$(resolve_tool "$clang_tidy_name")"
cppcheck_value="$(resolve_tool "$cppcheck_name")"

python3 - "$cc_value" "$format_value" "$tidy_value" "$cppcheck_value" <<'PY'
from pathlib import Path
import sys

cc, clang_format, clang_tidy, cppcheck = sys.argv[1:5]
updates = {
    "CC": cc,
    "CLANG_FORMAT": clang_format,
    "CLANG_TIDY": clang_tidy,
    "CPPCHECK": cppcheck,
}
changed = 0

for path in Path(".").rglob("Makefile"):
    text = path.read_text(encoding="utf-8", errors="replace")
    out: list[str] = []
    touched = False
    for line in text.splitlines():
        replaced = False
        for key, value in updates.items():
            if line.startswith(f"{key}?=") or line.startswith(f"{key}="):
                out.append(f"{key}?={value}")
                touched = True
                replaced = True
                break
        if not replaced:
            out.append(line)
    new_text = "\n".join(out) + ("\n" if text.endswith("\n") else "")
    if touched and new_text != text:
        path.write_text(new_text, encoding="utf-8")
        changed += 1

print(f"Updated {changed} Makefile(s).")
PY

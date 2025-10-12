#!/usr/bin/env bash
# check-compilers.sh — discover working C/C++ compilers on PATH
# Writes: supported_c_compilers.txt, supported_cxx_compilers.txt

set -euo pipefail

OS="$(uname -s)"

# Candidate names (most specific first)
c_compilers=(gcc15 gcc-15 gcc14 gcc-14 gcc13 gcc-13 clang-21 clang-20 clang-19 clang-18 clang-17 clang-16 clang-15 clang21 clang20 clang19 clang18 clang17 clang16 clang15 clang clang-devel)
cxx_compilers=(g++15 g++-15 g++14 g++-14 g++13 g++-13 clang++-21 clang++-20 clang++-19 clang++-18 clang++-17 clang++-16 clang++-15 clang++21 clang++20 clang++19 clang++18 clang++17 clang++16 clang++15 clang++ clang++-devel)

# On non-macOS, also consider generic gcc/g++
if [[ "$OS" != "Darwin" ]]; then
  c_compilers+=(gcc)
  cxx_compilers+=(g++)
fi

# De-dupe while preserving order
dedupe() {
  awk '!seen[$0]++' <(printf "%s\n" "$@")
}

# Filter out Apple stub gcc/g++ on macOS
filter_apple_stub() {
  local name path
  while IFS= read -r name; do
    [[ -n "$name" ]] || continue
    if path="$(command -v "$name" 2>/dev/null)"; then
      if [[ "$OS" == "Darwin" ]]; then
        if [[ "$path" == "/usr/bin/gcc" || "$path" == "/usr/bin/g++" ]]; then
          # Apple’s clang shim named gcc/g++; skip to avoid confusion
          continue
        fi
      fi
      printf '%s\n' "$name"
    fi
  done
}

# Can the compiler produce a trivial executable?
can_compile() {
  # can_compile <compiler-path> <lang>
  local cc="$1" lang="$2"
  local tmpdir src exe

  # Portable tmp dir
  tmpdir="$(mktemp -d 2>/dev/null || mktemp -d -t ccprobe)"
  # Ensure nounset does not trip when trap runs
  trap 'rm -rf "${tmpdir:-}" 2>/dev/null || true' RETURN

  src="$tmpdir/t.$lang"
  exe="$tmpdir/a.out"

  if [[ "$lang" == "c" ]]; then
    printf 'int main(void){return 0;}\n' >"$src"
  else
    printf 'int main(){return 0;}\n' >"$src"
  fi

  # Be quiet and avoid nonstandard libs
  if "$cc" -x "$lang" "$src" -o "$exe" >/dev/null 2>&1; then
    [[ -x "$exe" ]]
  else
    return 1
  fi
}

# Probe a list and write supported_<type>.txt atomically
# Args: <type> <lang> <candidates...>
probe_list() {
  local type="$1" lang="$2"; shift 2
  local out="supported_${type}.txt"
  local tmp

  # Portable tmp file path
  # On BSD/macOS, mktemp -t PREFIX prints a file path; on GNU, the template form works.
  tmp="$(mktemp "${out}.XXXXXX" 2>/dev/null || mktemp -t "${out%.txt}")"

  local name path ok=0
  while IFS= read -r name; do
    [[ -n "$name" ]] || continue
    path="$(command -v "$name" 2>/dev/null)" || continue
    if can_compile "$path" "$lang"; then
      printf '%s\n' "$name" >>"$tmp"
      ok=1
    fi
  done < <(dedupe "$@" | filter_apple_stub)

  # Always publish a file for reproducibility
  mv -f "$tmp" "$out"
  if [[ "$ok" -eq 0 ]]; then
    echo "No working ${type} found. Wrote empty ${out}." >&2
    exit 1
  fi
  echo "Supported ${type} compilers have been written to ${out}"
}

probe_list "c_compilers"   "c"   "${c_compilers[@]}"
probe_list "cxx_compilers" "c++" "${cxx_compilers[@]}"

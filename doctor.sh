#!/usr/bin/env bash
# doctor.sh — report what actually works on THIS machine for THIS project.
#
# Turns the pipeline's silent facts into a readable report: which tools are
# present (and whether the REQUIRED ones for a build are), and — per compiler —
# what the probe actually found for coverage, profiling and each sanitizer.
# It does NOT re-probe: it reads the buckets already written under .flags/ by
# ./update.sh, so an empty coverage_flags.txt shows up as "coverage n/a", a
# dropped leak sanitizer shows up as "leak n/a", etc. Read-only and safe.
#
# Platforms: macOS, Linux, FreeBSD.  Compilers: gcc and clang.
set -uo pipefail        # NOT -e: we run every check, then summarise.
CDPATH='' cd -- "$(dirname -- "${BASH_SOURCE[0]}")" || exit 1

usage() {
  cat <<'USAGE'
Usage: ./doctor.sh [--all]
  Reports, for this machine:
    * toolchain      — cmake, the supported compilers, and the analysis tools
                       the build needs (clang-tidy, cppcheck, clang-format, ...)
    * instrumentation— per compiler, what the probe found in .flags/<cc>/:
                       coverage, profiling (compile-time vs sampling), sanitizers

  With no arguments it reports the compiler this repo is currently configured
  with (from .last-build-dir); if nothing is configured yet it falls back to the
  first supported compiler found on PATH.

  --all   report the full supported-compiler matrix instead of just one.
  -h, --help   this help.

  Read-only: it never builds, probes, or modifies anything. Exit status is
  non-zero if a REQUIRED tool (cmake, clang-tidy, cppcheck) or the selected
  compiler is missing.
USAGE
}

show_all=0
case " $* " in
  *" --help "*|*" -h "*) usage; exit 0 ;;
  *" --all "*)           show_all=1 ;;
esac

OK="✓"; WARN="⚠"; NO="✗"
required_missing=0

have() { command -v "$1" >/dev/null 2>&1; }
firstline() { "$@" 2>/dev/null | head -1; }
# first version-looking token (N.N or N.N.N) anywhere in a tool's --version
# output. Portable (BSD/macOS + GNU): no sed 'T', no GNU-only flags.
ver_num() { "$@" 2>/dev/null | grep -oE '[0-9]+\.[0-9]+(\.[0-9]+)?' | head -1; }

# ---- locate config lists. In a fresh project instance these are expected at the repo
# root, usually as symlinks to a shared cache. Do not search parent directories:
# fresh template instances must not silently depend on the surrounding checkout layout.
find_list() {
  local n="$1" c
  for c in "." "scripts"; do
    if [ -f "$c/$n" ]; then echo "$c/$n"; return 0; fi
  done
}
_slist_c="$(find_list supported_c_compilers.txt)"
_slist_cxx="$(find_list supported_cxx_compilers.txt)"
_sanfile="$(find_list sanitizers.txt)"
_san_default=""
[ -n "$_sanfile" ] && _san_default="$(tr -d '[:space:]' < "$_sanfile")"

# basenames from a supported-compiler list (paths or bare names, '#' comments ok)
names_from() {
  [ -n "$1" ] && [ -f "$1" ] || return 0
  awk 'NF && $0 !~ /^[[:space:]]*#/ { n=split($0,a,"/"); print a[n] }' "$1"
}

# project language (C / CXX) from config.cmake, default C
project_lang() {
  if [ -f config.cmake ]; then
    sed -n 's/.*set(PROJECT_LANGUAGE[[:space:]]*"\{0,1\}\([A-Za-z]*\).*/\1/p' config.cmake | head -1
  fi
}

# derive the compiler basename this repo is configured with (authoritative:
# CMakeCache); empty if no configured build dir.
configured_cc() {
  local bd="" cc=""
  [ -f .last-build-dir ] && bd="$(cat .last-build-dir 2>/dev/null)"
  [ -n "$bd" ] || bd="build"
  if [ -f "$bd/CMakeCache.txt" ]; then
    cc="$(sed -n 's/^CMAKE_C_COMPILER:[^=]*=//p'   "$bd/CMakeCache.txt" | head -1)"
    [ -n "$cc" ] || cc="$(sed -n 's/^CMAKE_CXX_COMPILER:[^=]*=//p' "$bd/CMakeCache.txt" | head -1)"
  fi
  [ -n "$cc" ] && basename "$cc"
}

# matching gcov tool for a compiler basename (mirrors coverage-report.sh)
gcov_for() {
  case "$1" in
    gcc-*)  echo "gcov-${1#gcc-}" ;;
    g++-*)  echo "gcov-${1#g++-}" ;;
    *gcc*|*g++*) echo "gcov" ;;
    clang*) if have llvm-cov; then echo "llvm-cov gcov"; elif have xcrun; then echo "xcrun llvm-cov gcov"; else echo "gcov"; fi ;;
    *)      echo "gcov" ;;
  esac
}

line() { printf '%s\n' "------------------------------------------------------------"; }

# ============================ header ============================
os="$(uname -s 2>/dev/null || echo '?')"
arch="$(uname -m 2>/dev/null || echo '?')"
osname="$os"
case "$os" in
  Darwin) osname="macOS $(sw_vers -productVersion 2>/dev/null || echo '')" ;;
esac
here="$(basename "$(pwd)")"

printf 'p101 doctor — %s\n' "$here"
printf 'platform: %s (%s)\n' "$osname" "$arch"
if [ -L .flags ] || [ -d .flags ]; then
  printf 'flags:    %s\n' "$(cd .flags 2>/dev/null && pwd || echo '.flags')"
else
  printf 'flags:    %s .flags not found — run ./update.sh to probe this machine\n' "$WARN"
fi

# ========================= toolchain =========================
line; echo "toolchain"

# cmake (required)
if have cmake; then printf '  %s cmake            %s\n' "$OK" "$(firstline cmake --version | sed 's/cmake version //')"
else printf '  %s cmake            missing  [required]\n' "$NO"; required_missing=1; fi

# compilers from the supported lists (fall back to common names)
_ccs="$(names_from "$_slist_c")"
_cxxs="$(names_from "$_slist_cxx")"
[ -n "$_ccs" ]  || _ccs="gcc clang"
[ -n "$_cxxs" ] || _cxxs="g++ clang++"
echo "  compilers (supported):"
for c in $_ccs; do
  if have "$c"; then printf '    %s %-10s %s\n' "$OK" "$c" "$(firstline "$c" --version)"
  else printf '    %s %-10s not on PATH\n' "$NO" "$c"; fi
done
for c in $_cxxs; do
  if have "$c"; then printf '    %s %-10s %s\n' "$OK" "$c" "$(firstline "$c" --version)"
  else printf '    %s %-10s not on PATH\n' "$NO" "$c"; fi
done

# analysis tools the build uses
echo "  analysis:"
if have clang-tidy; then printf '    %s clang-tidy    %s  [required]\n' "$OK" "$(ver_num clang-tidy --version)"
else printf '    %s clang-tidy    missing  [required]\n' "$NO"; required_missing=1; fi

if have cppcheck; then
  _cpv="$(firstline cppcheck --version | sed 's/Cppcheck //')"
  if cppcheck --help 2>/dev/null | grep -q -- '--check-level'; then _cl="exhaustive: yes"; else _cl="exhaustive: no (cppcheck <2.11)"; fi
  printf '    %s cppcheck      %s  [required] (%s)\n' "$OK" "$_cpv" "$_cl"
else printf '    %s cppcheck      missing  [required]\n' "$NO"; required_missing=1; fi

if have clang-format; then printf '    %s clang-format  %s  -> ./build.sh --format enabled\n' "$OK" "$(ver_num clang-format --version)"
else printf '    %s clang-format  missing  (optional; ./build.sh --format skips formatting)\n' "$WARN"; fi

# ===================== instrumentation =====================
report_instr() {
  local cc="$1" d=".flags/$1"
  line
  printf 'instrumentation — %s\n' "$cc"
  if ! have "$cc"; then
    printf '  %s %s not on PATH — cannot use here\n' "$NO" "$cc"
    required_missing=1
  fi
  if [ ! -d "$d" ]; then
    printf '  %s no probed flags in .flags/%s — run ./update.sh -c %s\n' "$WARN" "$cc" "$cc"
    return 0
  fi

  # coverage
  if [ -s "$d/coverage_flags.txt" ]; then
    local mcdc=""
    grep -q -- '-fcondition-coverage' "$d/coverage_flags.txt" && mcdc="  + MC/DC (--conditions)"
    printf '  coverage    %s  gcov tool: %s%s\n' "$OK" "$(gcov_for "$cc")" "$mcdc"
  else
    printf '  coverage    %s  not available for %s here\n' "$NO" "$cc"
  fi

  # profiling (compile-time -pg) + sampling backend
  if [ -s "$d/profile_flags.txt" ]; then
    printf '  profiling   %s  compile-time (-pg) -> gprof\n' "$OK"
  else
    printf '  profiling   %s  compile-time (-pg) unavailable here — sampling only\n' "$WARN"
  fi
  case "$os" in
    Darwin) if have xctrace; then printf '              sampling: %s xctrace (Instruments) via ./report.sh profile\n' "$OK"
            elif have sample; then printf '              sampling: %s sample via ./report.sh profile\n' "$OK"
            else printf '              sampling: %s install Xcode / Command Line Tools\n' "$NO"; fi ;;
    Linux)  if have perf; then printf '              sampling: %s perf via ./report.sh profile\n' "$OK"
            else printf '              sampling: %s perf not installed (apt install linux-tools-...)\n' "$NO"; fi ;;
    FreeBSD) if have pmcstat; then printf '              sampling: %s pmcstat via ./report.sh profile\n' "$OK"
             else printf '              sampling: %s pmcstat not installed/available\n' "$NO"; fi ;;
    *)      printf '              sampling: %s unsupported platform\n' "$NO" ;;
  esac

  # sanitizers — partition every probed *_sanitizer_flags.txt into avail / n-a
  local avail="" na="" f nm
  shopt -s nullglob
  for f in "$d"/*_sanitizer_flags.txt; do
    nm="$(basename "$f")"; nm="${nm%_sanitizer_flags.txt}"
    if [ -s "$f" ]; then avail="$avail $nm"; else na="$na $nm"; fi
  done
  shopt -u nullglob
  printf '  sanitizers  avail:%s\n' "${avail:-  (none — run ./update.sh)}"
  [ -n "$na" ] && printf '              n/a  :%s\n' "$na"
  [ -n "$_san_default" ] && printf '              default selection: %s\n' "$_san_default"
}

if [ "$show_all" -eq 1 ]; then
  # matrix over the SUPPORTED compilers only (C list, plus the C++ list when it
  # is actually present). We deliberately do NOT walk every .flags/<cc> dir:
  # those accumulate stale entries (old compiler versions) that are just noise.
  _all_list="$_ccs"
  [ -n "$_slist_cxx" ] && _all_list="$_all_list $_cxxs"
  _seen=""
  for c in $_all_list; do
    case " $_seen " in *" $c "*) continue ;; esac
    report_instr "$c"; _seen="$_seen $c"
  done
else
  cc="$(configured_cc)"
  if [ -z "$cc" ]; then
    # nothing configured — fall back to first supported compiler on PATH
    lang="$(project_lang)"; [ -n "$lang" ] || lang="C"
    if [ "$lang" = "CXX" ] || [ "$lang" = "CPP" ]; then _pref="$_cxxs"; else _pref="$_ccs"; fi
    for c in $_pref; do if have "$c"; then cc="$c"; break; fi; done
    line
    printf '%s no build configured (run ./change-compiler.sh) — showing %s\n' "$WARN" "${cc:-<none>}"
  fi
  if [ -n "$cc" ]; then report_instr "$cc"; else
    line; printf '%s no usable compiler found on PATH\n' "$NO"; required_missing=1
  fi
fi

# ========================= verdict =========================
line
if [ "$required_missing" -eq 0 ]; then
  echo "verdict: required toolchain present."
  exit 0
else
  echo "verdict: MISSING a required tool/compiler (see ✗ above) — a build will not configure until fixed."
  case "$os" in
    Darwin) echo "         install hints: brew install cmake llvm cppcheck   (clang-tidy/clang-format ship with llvm)" ;;
    Linux)  echo "         install hints: apt install cmake clang-tidy clang-format cppcheck" ;;
    *)      echo "         install the missing tools with your platform package manager." ;;
  esac
  exit 1
fi

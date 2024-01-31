#!/usr/bin/env bash

# Exit the script if any command fails
set -e

# Function to detect system architecture
detect_architecture()
{
    local sys_name
    local architecture

    sys_name=$(uname -s)
    architecture=$(uname -m)

    if [[ $sys_name == "Darwin" ]]; then
        case $architecture in
            "arm64") echo "arm64";;
            "x86_64") echo "x86_64";;
            *) echo "unknown";;
        esac
    else
        echo "not_darwin"
    fi
}

# Function to check if a flag is supported by the compiler
is_flag_supported()
{
    local compiler="$1"
    local flag="$2"
    local supported_flags_ref="$3"

    local extra_flags=""

    # If the compiler is Clang, set the extra flags
    if [[ "$compiler" == clang* ]]; then
        extra_flags="-Winvalid-command-line-argument -Wunused-command-line-argument"
    fi

    # Check if the flag is supported by the compiler, including extra flags if set
    if ("$compiler" "$flag" $extra_flags -Werror -E - < /dev/null &> /dev/null); then
        echo "Flag '$flag' is supported by $compiler."
        eval "$supported_flags_ref+=('$flag')"
    else
        echo "Flag '$flag' is not supported by $compiler."
    fi
}

# Function to process compiler flags
process_compiler_flags()
{
    local compiler="$1"
    local category="$2"
    shift 2
    local flags=("$@")
    local supported_flags=()

    for flag in "${flags[@]}"; do
        is_flag_supported "$compiler" "$flag" supported_flags
    done

    # Concatenate the flags
    local flags_string
    flags_string=$(IFS=" "; echo "${supported_flags[*]}")

    # Write to file
    printf "%s" "$flags_string" > "./.flags/${compiler}/${category}_flags.txt"
}

process_sanitizer_category()
{
    local compiler="$1"
    local category_name="$2"
    local flags_array_name="$3"
    local supported_flags=()

    # Convert the string into an actual array
    eval "local flags_array=(\"\${${flags_array_name}[@]}\")"

    # Check the first flag
    is_flag_supported "$compiler" "${flags_array[0]}" supported_flags

    # Then, check the rest of the flags
    for i in "${!flags_array[@]}"; do
        if [[ $i -ne 0 ]]; then  # Skip the first element
            is_flag_supported "$compiler" "${flags_array[$i]}" supported_flags
        fi
    done

    # Concatenate the supported flags
    local flags_string
    flags_string=$(IFS=" "; echo "${supported_flags[*]}")

    # Write to file
    printf "%s" "$flags_string" > "./.flags/${compiler}/${category_name}_flags.txt"
}

# Main processing function
process_flags()
{
    local compiler="$1"
    local darwin_architecture="$2"
    local language="$3"

    # https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Static-Analyzer-Options.html
    local analyzer_flags=(
        "-fanalyzer"
        "-Wanalyzer-allocation-size"
        "-Wanalyzer-deref-before-check"
        "-Wanalyzer-double-fclose"
        "-Wanalyzer-double-free"
        "-Wanalyzer-exposure-through-output-file"
        "-Wanalyzer-exposure-through-uninit-copy"
        "-Wanalyzer-fd-access-mode-mismatch"
        "-Wanalyzer-fd-double-close"
        "-Wanalyzer-fd-leak"
        "-Wanalyzer-fd-phase-mismatch"
        "-Wanalyzer-fd-type-mismatch"
        "-Wanalyzer-fd-use-after-close"
        "-Wanalyzer-fd-use-without-check"
        "-Wanalyzer-file-leak"
        "-Wanalyzer-free-of-non-heap"
        "-Wanalyzer-imprecise-fp-arithmetic"
        "-Wanalyzer-infinite-recursion"
        "-Wanalyzer-jump-through-null"
        "-Wanalyzer-malloc-leak"
        "-Wanalyzer-mismatching-deallocation"
        "-Wanalyzer-null-argument"
        "-Wanalyzer-null-dereference"
        "-Wanalyzer-out-of-bounds"
        "-Wanalyzer-possible-null-argument"
        "-Wanalyzer-possible-null-dereference"
        "-Wanalyzer-putenv-of-auto-var"
        "-Wanalyzer-shift-count-negative"
        "-Wanalyzer-shift-count-overflow"
        "-Wanalyzer-stale-setjmp-buffer"
        "-Wanalyzer-symbol-too-complex"
#        "-Wanalyzer-too-complex"
        "-fanalyzer-transitivity"
        "-Wanalyzer-unsafe-call-within-signal-handler"
        "-Wanalyzer-use-after-free"
        "-Wanalyzer-use-of-pointer-in-stale-stack-frame"
        "-Wanalyzer-use-of-uninitialized-value"
        "-Wanalyzer-va-arg-type-mismatch"
        "-Wanalyzer-va-list-exhausted"
        "-Wanalyzer-va-list-leak"
        "-Wanalyzer-va-list-use-after-va-end"
        "-fanalyzer-verbosity=3"
        "-Wanalyzer-write-to-const"
        "-Wanalyzer-write-to-string-literal"
    )

    # https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Debugging-Options.html
    local debug_flags=(
        "-g"
        "-g3"
        "-ggdb"
        "-ggdb3"
        "-fvar-tracking"
        "-fvar-tracking-assignments"
        "-gdescribe-die"
        "-gpubnames"
        "-ggnu-pubnames"
        "-fdebug-types-section"
        "-grecord-gcc-switches"
        "-gas-loc-support"
#        "-gas-locview-support"
        "-gcolumn-info"
        "-gstatement-frontiers"
        "-gvariable-location-views"
        "-ginternal-reset-location-views"
        "-ginline-points"
        "-feliminate-unused-debug-types"
        #"-fdebug-macro"
        "-glldb"
        "-fno-discard-value-names"
    )
    ##-femit-class-debug-always

    local optimization_flags=(
      "-O0"
    )

    # https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Optimize-Options.html
    local optimize_flags=(
        "-fno-fast-math"
        "-fstrict-float-cast-overflow"
        "-fmath-errno"
        "-ftrapping-math"
        "-fhonor-infinities"
        "-fhonor-nans"
        "-fnoapprox-func"
        "-fsigned-zeros"
        "-fno-associative-math"
        "-fno-reciprocal-math"
        "-fno-unsafe-math-optimizations"
        "-fnofinite-math-only"
        "-frounding-math"
        "-ffp-model=strict"
        "-ffp-exception-behavior=strict"
        "-ffp-eval-method=source"
        "-fprotect-parens"
        "-fexcess-precision:standard"
        "-fno-cx-limited-range"
        "-fno-cx-fortran-rules"
    )

    local instrumentation_flags=(
      "-faddrsig"
      "-fbasic-block-sections=all"
      "-fharden-compares"
      "-fharden-conditional-branches"
      "-fno-sanitize-ignorelist"
      "-fno-sanitize-recover=all"
      "-fno-stack-limit"
      "-fsanitize=shadow-call-stack"
      "-fsanitize-trap=all"
      "-fsanitize=unreachable"
      "-fstack-check"
#      "-fstack-clash-protection"
      "-fstack-protector"
      "-fstack-protector-all"
      "-fstack-protector-strong"
      "-ftls-model=global-dynamic"
      "-funique-internal-linkage-names"

      #"-fsanitize=object-size"
      #"-fsanitize-coverage=trace-pc"
      #"-fsplit-stack"
      #-f[no-]sanitize-coverage=
      #-f[no-]sanitize-address-outline-instrumentation
      #-f[no-]sanitize-stats
      #-femulated-tls
      #-mhwdiv=
      #-m[no-]crc
      #-mgeneral-regs-only
      ###-Winterference-size
    )

    # https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Warning-Options.html
    # https://clang.llvm.org/docs/DiagnosticsReference.html#wanalyzer-incompatible-plugin
    local warning_flags=(
      "-Wno-poison-system-directories"
      "-Wno-invalid-command-line-argument"
      "-Wno-unused-command-line-argument"
      "-Werror"
      #"-Wfatal-errors"
      "-Wpedantic"
      "-pedantic"
      "-pedantic-errors"
      "-Wall"
      "-Wextra"
      "-Wabi"
      "-Wchar-subscripts"
      "-Wdouble-promotion"
      "-Wformat=2"
      "-Wformat-overflow=2"
      "-Wformat-nonliteral"
      "-Wformat-security"
      "-Wformat-signedness"
      "-Wformat-truncation=2"
      "-Wformat-y2k"
      "-Wnonnull"
      "-Wnonnull-compare"
      "-Wnull-dereference"
      "-Winfinite-recursion"
      "-Winit-self"
      "-Wimplicit-fallthrough=3"
      "-Wignored-qualifiers"
      "-Wmain"
      "-Wmisleading-indentation"
      "-Wmissing-attributes"
      "-Wmissing-braces"
      "-Wmissing-include-dirs"
      "-Wmismatched-dealloc"
      "-Wmultistatement-macros"
      "-Wparentheses"
      "-Wsequence-point"
      "-Wreturn-type"
      "-Wshift-negative-value"
      "-Wshift-overflow=2"
      "-Wswitch"
      "-Wswitch-default"
      "-Wswitch-enum"
      "-Wsync-nand"
      "-Wtrivial-auto-var-init"
      "-Wunused-but-set-parameter"
      "-Wunused-but-set-variable"
      "-Wunused-function"
      "-Wunused-label"
      "-Wunused-local-typedefs"
      "-Wunused-parameter"
      "-Wno-unused-result"
      "-Wunused-variable"
      "-Wunused-const-variable=2"
      "-Wunused-value"
      "-Wunused"
      "-Wuninitialized"
      "-Wmaybe-uninitialized"
      "-Wunknown-pragmas"
      "-Wstrict-aliasing"
#      "-Wstrict-overflow=5"
      "-Wstring-compare"
      "-Wstringop-overflow=4"
      "-Wstrict-flex-arrays"
      "-Wsuggest-attribute=pure"
      "-Wsuggest-attribute=const"
      "-Wsuggest-attribute=noreturn"
      "-Wmissing-noreturn"
      "-Wsuggest-attribute=malloc"
      "-Wsuggest-attribute=returns_nonnull"
      "-Wno-suggest-attribute=returns_nonnull"
      "-Wsuggest-attribute=format"
      "-Wmissing-format-attribute"
      "-Wsuggest-attribute=cold"
      "-Walloc-zero"
      "-Walloca"
      "-Warith-conversion"
      "-Warray-bounds=2"
      "-Warray-compare"
      "-Warray-parameter=2"
      "-Wattribute-alias=2"
      "-Wbidi-chars=any"
      "-Wbool-compare"
      "-Wbool-operation"
      "-Wduplicated-branches"
      "-Wduplicated-cond"
      "-Wframe-address"
      "-Wzero-length-bounds"
      "-Wtautological-compare"
      "-Wtrampolines"
      "-Wfloat-equal"
      "-Wshadow"
      "-Wshadow=global"
      "-Wfree-nonheap-object"
      "-Wunsafe-loop-optimizations"
      "-Wpointer-arith"
      "-Wtsan"
      "-Wtype-limits"
      "-Wcomment"
      "-Wcomments"
      "-Wtrigraphs"
      "-Wundef"
      "-Wunused-macros"
      "-Wcast-qual"
      "-Wcast-align=strict"
      "-Wcast-function-type"
      "-Wwrite-strings"
      "-Wclobbered"
      "-Wconversion"
      "-Wdangling-else"
      "-Wdangling-pointer=2"
      "-Wdate-time"
      "-Wempty-body"
      "-Wenum-compare"
      "-Wenum-conversion"
      "-Wsign-compare"
      "-Wsign-conversion"
      "-Wfloat-conversion"
      "-Wsizeof-array-div"
      "-Wsizeof-pointer-div"
      "-Wsizeof-pointer-memaccess"
      "-Wmemset-elt-size"
      "-Wmemset-transposed-args"
      "-Waddress"
      "-Wlogical-op"
      "-Wlogical-not-parentheses"
      "-Waggregate-return"
      "-Wmissing-declarations"
      "-Wmissing-field-initializers"
      "-Wnormalized=nfc"
      "-Wopenacc-parallelism"
      "-Wopenmp-simd"
      "-Wpacked"
      "-Wpacked-not-aligned"
      #"-Wpadded"
      "-Wno-padded"
      "-Wredundant-decls"
      "-Wrestrict"
      "-Winline"
      "-Wint-in-bool-context"
      "-Winvalid-pch"
      "-Winvalid-utf8"
      # "-Wlong-long"
      "-Wvector-operation-performance"
      "-Wvla"
      "-Wvla-parameter"
      "-Wvolatile-register-var"
      "-Wxor-used-as-pow"
      "-Wdisabled-optimization"
      "-Woverlength-strings"
      "-Wextra-tokens"
      "-Weverything"
      "-Wno-unsafe-buffer-usage"
      ###-Wstack-protector
      "-W#pragma-messages"
      "-W#warnings"
      "-WCFString-literal"
      "-WCL4"
      "-Waddress-of-packed-member"
      "-Waddress-of-temporary"
      "-Walign-mismatch"
      "-Walloca-with-align-alignof"
      "-Walways-inline-coroutine"
      "-Wambiguous-ellipsis"
      "-Wambiguous-macro"
      "-Wanalyzer-incompatible-plugin"
      "-Wanon-enum-enum-conversion"
      "-Wapinotes"
      "-Wargument-outside-range"
      "-Wargument-undefined-behaviour"
      "-Warray-bounds-pointer-arithmetic"
      "-Wasm-operand-widths"
      "-Wassign-enum"
      "-Wassume"
      "-Wat-protocol"
      "-Watimport-in-framework-header"
      "-Watomic-access"
      "-Watomic-alignment"
      "-Watomic-implicit-seq-cst"
      "-Watomic-memory-ordering"
      "-Watomic-properties"
      "-Wattribute-packed-for-bitfield"
      "-Wattribute-warning"
      "-Wattributes"
      "-Wauto-decl-extensions"
      "-Wauto-import"
      "-Wavailability"
      "-Wavr-rtlib-linking-quirks"
      "-Wbackend-plugin"
      "-Wbackslash-newline-escape"
      "-Wbinary-literal"
      "-Wbit-int-extension"
      "-Wbitfield-constant-conversion"
      "-Wbitfield-enum-conversion"
      "-Wbitfield-width"
      "-Wbitwise-conditional-parentheses"
      "-Wbitwise-instead-of-logical"
      "-Wbitwise-op-parentheses"
      "-Wblock-capture-autoreleasing"
      "-Wbool-conversion"
      "-Wbool-operation"
      "-Wbraced-scalar-init"
      "-Wbranch-protection"
      "-Wbridge-cast"
      "-Wbuiltin-assume-aligned-alignment"
      "-Wbuiltin-macro-redefined"
      "-Wbuiltin-memcpy-chk-size"
      "-Wbuiltin-requires-header"
      "-Wcalled-once-parameter"
      "-Wcast-calling-convention"
      "-Wcast-function-type-strict"
      "-Wcast-of-sel-type"
      "-Wcast-qual-unrelated"
    )

    # https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Instrumentation-Options.html
    local address_sanitizer_flags=(
        "-fsanitize=address"
        "-fsanitize-address-use-after-scope"
        "-fsanitize=leak"
    )

    local cfi_sanitizer_flags=(
        "-fsanitize=cfi"
        "-fsanitize-cfi-cross-dso"
        "-fsanitize-cfi-icall-generalize-pointers"
        "-fsanitize-cfi-icall-experimental-normalize-integers"
    )

    local dataflow_sanitizer_flags=(
        "-fsanitize=dataflow"
    )

    local hwaddress_sanitizer_flags=(
        "-fsanitize=hwaddress"
    )

    local memory_sanitizer_flags=(
        "-fsanitize=memory"
    )

    local pointer_overflow_sanitizer_flags=(
          "-fsanitize=pointer-overflow"
      )

    local safe_stack_flags=(
          "--fsanitize=safe-stack"
      )

    local thread_sanitizer_flags=(
          "-fsanitize=thread"
      )

    local undefined_sanitizer_flags=(
        "-fsanitize=undefined"
        "-fsanitize=alignment"
        "-fsanitize=bool"
        "-fsanitize=bounds"
        "-fsanitize=bounds-strict"
        "-fsanitize=builtin"
        "-fsanitize=enum"
        "-fsanitize=float-cast-overflow"
        "-fsanitize=float-divide-by-zero"
        "-fsanitize=function"
        "-fsanitize=integer"
        "-fsanitize=integer-divide-by-zero"
        "-fsanitize=nonnull-attribute"
        "-fsanitize=null"
        "-fsanitize=pointer-compare"
        "-fsanitize=pointer-subtract"
        "-fsanitize=return"
        "-fsanitize=returns-nonnull-attribute"
        "-fsanitize=shift"
        "-fsanitize=shift-exponent"
        "-fsanitize=shift-base"
        "-fsanitize=signed-integer-overflow"
        "-fsanitize-undefined-trap-on-error"
        "-fsanitize=vla-bound"
    )

    if [[ $language == "c" ]]; then
        warning_flags+=("-Wbad-function-cast")
        warning_flags+=("-Wdeclaration-after-statement")
        warning_flags+=("-Wenum-int-mismatch")
        warning_flags+=("-Wimplicit")
        warning_flags+=("-Wjump-misses-init")
        warning_flags+=("-Wmissing-parameter-type")
        warning_flags+=("-Wmissing-prototypes")
        warning_flags+=("-Wnested-externs")
        warning_flags+=("-Wold-style-declaration")
        warning_flags+=("-Wold-style-definition")
        warning_flags+=("-Wpointer-sign")
        warning_flags+=("-Wstrict-prototypes")
        warning_flags+=("-Wc++-compat")
        warning_flags+=("-Wabsolute-value")
        warning_flags+=("-Wduplicate-decl-specifier")
        warning_flags+=("-Wimplicit-function-declaration")
        warning_flags+=("-Wimplicit-int")
        warning_flags+=("-Wincompatible-pointer-types")
        warning_flags+=("-Wint-conversion")
        warning_flags+=("-Woverride-init")
        warning_flags+=("-Wpointer-to-int-cast")
    else
        warning_flags+=("-Wambiguous-member-template")
        warning_flags+=("-Wbind-to-temporary-copy")
        warning_flags+=("-Wabstract-final-class")
        warning_flags+=("-Wabstract-vbase-init")
        warning_flags+=("-Wambiguous-delete")
        warning_flags+=("-Wambiguous-reversed-operator")
        warning_flags+=("-Wanonymous-pack-parens")
        warning_flags+=("-Wauto-disable-vptr-sanitizer")
        warning_flags+=("-Wauto-storage-class")
        warning_flags+=("-Wauto-var-id")
        warning_flags+=("-Wbind-to-temporary-copy")
        warning_flags+=("-Wbinding-in-condition")
        warning_flags+=("-Wcall-to-pure-virtual-from-ctor-dtor")

        # C++ options
        #instrumentation_flags+=("-fsanitize=vptr")

        # VTable
        #instrumentation_flags+=("-fvtable-verify=preinit")
        instrumentation_flags+=("-fvtv-debug")
        instrumentation_flags+=("-fstrict-vtable-pointers")
        instrumentation_flags+=("-fwhole-program-vtables")
        instrumentation_flags+=("-fforce-emit-vtables")

        # linker
        #instrumentation_flags+=("-f[no]split-lto-unit")

        # memory options
        instrumentation_flags+=("-fno-assume-sane-operator-new")
        instrumentation_flags+=("-fassume-nothrow-exception-dtor")
    fi

    if [[ $compiler == "arm64" ]]; then
        instrumentation_flags+=("-fcf-protection=null")
    else
        instrumentation_flags+=("-fcf-protection=full")
    fi

    echo "Checking: $compiler"

    # Prepare directory
    local flag_dir="./.flags/${compiler}"
    mkdir -p "$flag_dir"
    rm -f "$flag_dir"/*

    # Process each flag category
    process_compiler_flags "$compiler" "analyzer" "${analyzer_flags[@]}"
    process_compiler_flags "$compiler" "debug" "${debug_flags[@]}"
    process_compiler_flags "$compiler" "instrumentation" "${instrumentation_flags[@]}"
    process_compiler_flags "$compiler" "optimization" "${optimization_flags[@]}"
    process_compiler_flags "$compiler" "optimize" "${optimize_flags[@]}"
    process_compiler_flags "$compiler" "warning" "${warning_flags[@]}"
    process_sanitizer_category "$compiler" "address_sanitizer" "address_sanitizer_flags"
    process_sanitizer_category "$compiler" "cfi_sanitizer" "cfi_sanitizer_flags"
    process_sanitizer_category "$compiler" "dataflow_sanitizer" "dataflow_sanitizer_flags"
    process_sanitizer_category "$compiler" "hwaddress_sanitizer" "hwaddress_sanitizer_flags"
    process_sanitizer_category "$compiler" "memory_sanitizer" "memory_sanitizer_flags"
    process_sanitizer_category "$compiler" "pointer_overflow_sanitizer" "pointer_overflow_sanitizer_flags"
    process_sanitizer_category "$compiler" "safe_stack_flags" "safe_stack_flags"
    process_sanitizer_category "$compiler" "thread_sanitizer_flags" "thread_sanitizer_flags"
    process_sanitizer_category "$compiler" "undefined_sanitizer" "undefined_sanitizer_flags"
}

darwin_architecture=$(detect_architecture)

if [ ! -f "supported_c_compilers.txt" ]; then
    # File does not exist, execute 'check-compilers.sh'
    ./check-compilers.sh
fi

# Read the list of supported compilers and process each
supported_c_compilers=()
while IFS= read -r line; do
    supported_c_compilers+=("$line")
done < supported_c_compilers.txt

# Process C++ compilers
for compiler in "${supported_c_compilers[@]}"; do
    process_flags "$compiler" "$darwin_architecture" "cxx"
done

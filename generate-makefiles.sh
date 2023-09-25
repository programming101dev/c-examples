#!/usr/bin/env bash

# Function to determine the platform and set the shared library extension accordingly
get_shared_lib_extension() {
    case "$(uname -s)" in
        Linux) SHARED_EXT=".so" ;;
        Darwin) SHARED_EXT=".dylib" ;;
        FreeBSD) SHARED_EXT=".so" ;;
        *)
            echo "Unsupported platform"
            exit 1
            ;;
    esac
}

# Function to check if a flag is supported, considering flag dependencies
is_flag_supported() {
    local flag="$1"
    local dependencies="$2"  # Additional dependencies to consider

    # Check if dependencies are provided and, if so, add them to the flag
    if [[ -n $dependencies ]]; then
        flag="$flag $dependencies"
    fi

    echo "int main(void) { return 0; }" > test.c
    local output="$($CC -Werror $flag -E -o /dev/null test.c 2>&1)"
    local result=$?
    rm test.c 2>/dev/null

    if [[ $result -eq 0 ]] && [[ -z $output ]]; then
        return 0
    else
        echo "Flag '$flag' is NOT supported."
        if [[ -n $output ]]; then
            echo "Compiler error message:"
            echo "$output"
        fi
        return 1
    fi
}

# Flags to test and use (global variable)
SUPPORTED_FLAGS=()

# Function to populate the SUPPORTED_FLAGS array
populate_supported_flags() {
    # Flags to test and use
    WARNING_FLAGS=(
      "-pedantic-errors"
      "-Waddress"
      "-Waggregate-return"
      "-Wall"
      "-Walloc-zero"
      "-Walloca"
      "-Warith-conversion"
      "-Warray-bounds"
      "-Wattribute-alias=2"
      "-Wbad-function-cast"
      "-Wbool-compare"
      "-Wbool-operation"
      "-Wcast-align"
      "-Wcast-align=strict"
      "-Wcast-function-type"
      "-Wcast-qual"
      "-Wchar-subscripts"
      "-Wclobbered"
      "-Wcomment"
      "-Wconversion"
      "-Wdangling-else"
      "-Wdangling-pointer"
      "-Wdangling-pointer=2"
      "-Wdate-time"
      "-Wdeclaration-after-statement"
      "-Wdisabled-optimization"
      "-Wdouble-promotion"
      "-Wduplicated-branches"
      "-Wduplicated-cond"
      "-Wempty-body"
      "-Wenum-compare"
      "-Wenum-conversion"
      "-Wenum-int-mismatch"
      "-Wexpansion-to-defined"
      "-Wextra"
      "-Wfatal-errors"
      "-Wflex-array-member-not-at-end"
      "-Wfloat-conversion"
      "-Wfloat-equal"
      "-Wformat"
      "-Wformat-overflow=2"
      "-Wformat-security"
      "-Wformat-signedness"
      "-Wformat-truncation=2"
      "-Wformat-y2k"
      "-Wformat=2"
      "-Wframe-address"
      "-Wignored-qualifiers"
      "-Wimplicit"
      "-Wimplicit-fallthrough"
      "-Wimplicit-fallthrough=3"
      "-Winfinite-recursion"
      "-Winit-self"
      "-Winline"
      "-Wint-in-bool-context"
      "-Winvalid-pch"
      "-Winvalid-utf8"
      "-Wjump-misses-init"
      "-Wlogical-not-parentheses"
      "-Wlogical-op"
      "-Wmain"
      "-Wmaybe-uninitialized"
      "-Wmemset-elt-size"
      "-Wmemset-transposed-args"
      "-Wmisleading-indentation"
      "-Wmissing-attributes"
      "-Wmissing-braces"
      "-Wmissing-declarations"
      "-Wmissing-field-initializers"
      "-Wmissing-format-attribute"
      "-Wmissing-include-dirs"
      "-Wmissing-noreturn"
      "-Wmissing-parameter-type"
      "-Wmissing-prototypes"
      "-Wmissing-variable-declarations"
      "-Wmultistatement-macros"
      "-Wnested-externs"
      "-Wnull-dereference"
      "-Wold-style-declaration"
      "-Wold-style-definition"
      "-Wopenacc-parallelism"
      "-Wopenmp-simd"
      "-Woverlength-strings"
      "-Wpacked"
      "-Wpacked-not-aligned"
#      "-Wpadded"
      "-Wparentheses"
      "-Wpedantic"
      "-Wpointer-arith"
      "-Wpointer-sign"
      "-Wredundant-decls"
      "-Wrestrict"
      "-Wreturn-type"
      "-Wsequence-point"
      "-Wshadow"
      "-Wshadow=compatible-local"
      "-Wshadow=global"
      "-Wshadow=local"
      "-Wshift-negative-value"
      "-Wshift-overflow=2"
      "-Wsign-compare"
      "-Wsign-conversion"
      "-Wsizeof-array-div"
      "-Wsizeof-pointer-div"
      "-Wsizeof-pointer-memaccess"
      "-Wstack-protector"
      "-Wstrict-aliasing"
      "-Wstrict-aliasing=3"
      "-Wstrict-flex-arrays"
#      "-Wstrict-overflow"
#      "-Wstrict-overflow=4"
      "-Wstrict-prototypes"
      "-Wstring-compare"
      "-Wswitch"
      "-Wswitch-default"
      "-Wswitch-enum"
      "-Wsync-nand"
      "-Wtautological-compare"
      "-Wtrampolines"
      "-Wtrigraphs"
      "-Wtrivial-auto-var-init"
      "-Wtsan"
      "-Wtype-limits"
      "-Wundef"
      "-Wuninitialized"
      "-Wunknown-pragmas"
#      "-Wunsuffixed-float-constants"
      "-Wunused"
      "-Wunused-but-set-parameter"
      "-Wunused-but-set-variable"
      "-Wunused-const-variable"
      "-Wunused-const-variable=2"
      "-Wunused-function"
      "-Wunused-label"
      "-Wunused-local-typedefs"
      "-Wunused-macros"
      "-Wunused-parameter"
      "-Wunused-value"
      "-Wunused-variable"
      "-Wvariadic-macros"
      "-Wvector-operation-performance"
      "-Wvla"
      "-Wvolatile-register-var"
      "-Wwrite-strings"
      "-Wxor-used-as-pow"
      "-Wzero-length-bounds"
      "-Wbidi-chars=unpaired,ucn"
      "-Wc++-compat"
      "-W"
# these need special support for the makefile      "-W#pragma-messages"
# these need special support for the makefile      "-W#warnings"
#      "-Wabi"
      "-Wabsolute-value"
      "-Waddress-of-packed-member"
      "-Waddress-of-temporary"
      "-Waix-compat"
      "-Walign-mismatch"
      "-Walloca-with-align-alignof"
      "-Walways-inline-coroutine"
      "-Wambiguous-ellipsis"
      "-Wambiguous-macro"
      "-Wambiguous-member-template"
      "-Wambiguous-reversed-operator"
      "-Wanalyzer-incompatible-plugin"
      "-Wanon-enum-enum-conversion"
      "-Wanonymous-pack-parens"
      "-Warc"
      "-Warc-bridge-casts-disallowed-in-nonarc"
      "-Warc-maybe-repeated-use-of-weak"
      "-Warc-non-pod-memaccess"
      "-Warc-performSelector-leaks"
      "-Warc-repeated-use-of-weak"
      "-Warc-retain-cycles"
      "-Warc-unsafe-retained-assign"
      "-Wargument-outside-range"
      "-Wargument-undefined-behaviour"
      "-Warray-bounds-pointer-arithmetic"
      "-Warray-parameter"
      "-Wasm"
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
      "-Watomic-property-with-user-defined-accessor"
      "-Wattribute-packed-for-bitfield"
      "-Wattribute-warning"
      "-Wattributes"
      "-Wauto-disable-vptr-sanitizer"
      "-Wauto-import"
      "-Wauto-storage-class"
      "-Wauto-var-id"
      "-Wavailability"
      "-Wavr-rtlib-linking-quirks"
      "-Wbackend-plugin"
      "-Wbackslash-newline-escape"
      "-Wbinary-literal"
      "-Wbind-to-temporary-copy"
      "-Wbinding-in-condition"
      "-Wbit-int-extension"
      "-Wbitfield-constant-conversion"
      "-Wbitfield-enum-conversion"
      "-Wbitfield-width"
      "-Wbitwise-conditional-parentheses"
      "-Wbitwise-instead-of-logical"
      "-Wbitwise-op-parentheses"
      "-Wblock-capture-autoreleasing"
      "-Wbool-conversion"
      "-Wbool-conversions"
      "-Wbool-operation"
      "-Wbraced-scalar-init"
      "-Wbranch-protection"
      "-Wbridge-cast"
      "-Wbuiltin-assume-aligned-alignment"
      "-Wbuiltin-macro-redefined"
      "-Wbuiltin-memcpy-chk-size"
      "-Wbuiltin-requires-header"
      "-Wc11-extensions"
      "-Wc23-compat"
      "-Wc23-extensions"
      "-Wc2x-compat"
      "-Wc2x-extensions"
      "-Wc99-compat"
      "-Wc99-designator"
      "-Wc99-extensions"
      "-Wcalled-once-parameter"
      "-Wcast-calling-convention"
      "-Wcast-function-type-strict"
      "-Wcast-of-sel-type"
      "-Wcast-qual-unrelated"
      "-WCFString-literal"
      "-Wchar-align"
      "-WCL4"
      "-Wclang-cl-pch"
      "-Wclass-varargs"
      "-Wcmse-union-leak"
      "-Wcomma"
      "-Wcomments"
      "-Wcompletion-handler"
      "-Wcomplex-component-init"
      "-Wcompound-token-split"
      "-Wcompound-token-split-by-macro"
      "-Wcompound-token-split-by-space"
      "-Wconditional-type-mismatch"
      "-Wconditional-uninitialized"
      "-Wconfig-macros"
      "-Wconstant-conversion"
      "-Wconstant-evaluated"
      "-Wconstant-logical-operand"
      "-Wconstexpr-not-const"
      "-Wconsumed"
      "-Wcoro-non-aligned-allocation-function"
      "-Wcoroutine"
      "-Wcoroutine-missing-unhandled-exception"
      "-Wcovered-switch-default"
      "-Wcpp"
      "-Wcstring-format-directive"
      "-Wctu"
      "-Wcuda-compat"
      "-Wcustom-atomic-properties"
      "-Wcxx-attribute-extension"
      "-Wdangling"
      "-Wdangling-field"
      "-Wdangling-gsl"
      "-Wdangling-initializer-list"
      "-Wdealloc-in-category"
      "-Wdebug-compression-unavailable"
      "-Wdefaulted-function-deleted"
      "-Wdelegating-ctor-cycles"
      "-Wdelimited-escape-sequence-extension"
      "-Wdeprecate-lax-vec-conv-all"
      "-Wdeprecated"
      "-Wdeprecated-altivec-src-compat"
      "-Wdeprecated-anon-enum-enum-conversion"
      "-Wdeprecated-array-compare"
      "-Wdeprecated-attributes"
      "-Wdeprecated-builtins"
      "-Wdeprecated-comma-subscript"
      "-Wdeprecated-coroutine"
      "-Wdeprecated-declarations"
      "-Wdeprecated-dynamic-exception-spec"
      "-Wdeprecated-enum-compare"
      "-Wdeprecated-enum-compare-conditional"
      "-Wdeprecated-implementations"
      "-Wdeprecated-increment-bool"
      "-Wdeprecated-literal-operator"
      "-Wdeprecated-non-prototype"
      "-Wdeprecated-pragma"
      "-Wdeprecated-redundant-constexpr-static-def"
      "-Wdeprecated-register"
      "-Wdeprecated-static-analyzer-flag"
      "-Wdeprecated-this-capture"
      "-Wdeprecated-type"
      "-Wdeprecated-volatile"
      "-Wdeprecated-writable-strings"
      "-Wdirect-ivar-access"
      "-Wdisabled-macro-expansion"
      "-Wdiscard-qual"
      "-Wdistributed-object-modifiers"
      "-Wdiv-by-zero"
      "-Wdivision-by-zero"
      "-Wdll-attribute-on-redeclaration"
      "-Wdllexport-explicit-instantiation-decl"
      "-Wdllimport-static-field-def"
      "-Wdocumentation"
      "-Wdocumentation-deprecated-sync"
      "-Wdocumentation-html"
      "-Wdocumentation-pedantic"
      "-Wdocumentation-unknown-command"
      "-Wdollars-in-identifier-extension"
      "-Wdollar-in-identifier-extension"
      "-Wdtor-name"
      "-Wdtor-typedef"
      "-Wduplicate-decl-specifier"
      "-Wduplicate-enum"
      "-Wduplicate-method-arg"
      "-Wduplicate-method-match"
      "-Wduplicate-protocol"
      "-Wdxil-validation"
      "-Wdynamic-class-memaccess"
      "-Wdynamic-exception-spec"
      "-Weager-load-cxx-named-modules"
      "-Welaborated-enum-base"
      "-Welaborated-enum-class"
      "-Wembedded-directive"
      "-Wempty-decomposition"
      "-Wempty-init-stmt"
      "-Wempty-translation-unit"
      "-Wencode-type"
      "-Wendif-labels"
      "-Wenum-compare-conditional"
      "-Wenum-compare-switch"
      "-Wenum-constexpr-conversion"
      "-Wenum-enum-conversion"
      "-Wenum-float-conversion"
      "-Wenum-too-large"
      "-Wexcess-initializers"
      "-Wexcessive-regsave"
      "-Wexit-time-destructors"
      "-Wexperimental-header-units"
      "-Wexplicit-initialize-call"
      "-Wexplicit-ownership-type"
      "-Wexport-unnamed"
      "-Wextern-c-compat"
      "-Wextern-initializer"
      "-Wextra-qualification"
      "-Wextra-semi-stmt"
      "-Wextra-tokens"
      "-Wfinal-dtor-non-final-class"
      "-Wfinal-macro"
      "-Wfixed-enum-extension"
      "-Wfixed-point-overflow"
      "-Wflag-enum"
      "-Wflexible-array-extensions"
      "-Wfloat-overflow-conversion"
      "-Wfloat-zero-conversion"
      "-Wfor-loop-analysis"
      "-Wformat-extra-args"
      "-Wformat-insufficient-args"
      "-Wformat-invalid-specifier"
      "-Wformat-non-iso"
      "-Wformat-nonliteral"
      "-Wformat-pedantic"
      "-Wformat-type-confusion"
      "-Wformat-zero-length"
      "-Wfortify-source"
      "-Wfour-char-constants"
      "-Wframework-include-private-from-public"
      "-Wfree-nonheap-object"
      "-Wfunction-multiversion"
      "-Wfuse-ld-path"
      "-Wfuture-attribute-extensions"
      "-Wfuture-compat"
      "-Wgcc-compat"
      "-Wgeneric-type-extension"
      "-Wglobal-constructors"
      "-Wglobal-isel"
      "-Wgnu"
      "-Wgnu-alignof-expression"
      "-Wgnu-anonymous-struct"
      "-Wgnu-array-member-paren-init"
      "-Wgnu-auto-type"
      "-Wgnu-binary-literal"
      "-Wgnu-case-range"
      "-Wgnu-complex-integer"
      "-Wgnu-compound-literal-initializer"
      "-Wgnu-conditional-omitted-operand"
      "-Wgnu-designator"
      "-Wgnu-empty-initializer"
      "-Wgnu-empty-struct"
      "-Wgnu-flexible-array-initializer"
      "-Wgnu-flexible-array-union-member"
      "-Wgnu-folding-constant"
      "-Wgnu-imaginary-constant"
      "-Wgnu-include-next"
      "-Wgnu-inline-cpp-without-extern"
      "-Wgnu-label-as-value"
      "-Wgnu-line-marker"
      "-Wgnu-null-pointer-arithmetic"
      "-Wgnu-offsetof-extensions"
      "-Wgnu-pointer-arith"
      "-Wgnu-redeclared-enum"
      "-Wgnu-statement-expression"
      "-Wgnu-statement-expression-from-macro-expansion"
      "-Wgnu-static-float-init"
      "-Wgnu-string-literal-operator-template"
      "-Wgnu-union-cast"
      "-Wgnu-variable-sized-type-not-at-end"
      "-Wgnu-zero-line-directive"
      "-Wgnu-zero-variadic-macro-arguments"
      "-Wgpu-maybe-wrong-side"
      "-Wheader-guard"
      "-Wheader-hygiene"
      "-Whip-omp-target-directives"
      "-Whip-only"
      "-Whlsl-extensions"
      "-Widiomatic-parentheses"
      "-Wignored-attributes"
      "-Wignored-availability-without-sdk-settings"
      "-Wignored-optimization-argument"
      "-Wignored-pragma-intrinsic"
      "-Wignored-pragma-optimize"
      "-Wignored-pragmas"
      "-Wignored-reference-qualifiers"
      "-Wimplicit-atomic-properties"
      "-Wimplicit-const-int-float-conversion"
      "-Wimplicit-conversion-floating-point-to-bool"
      "-Wimplicit-exception-spec-mismatch"
      "-Wimplicit-fallthrough-per-function"
      "-Wimplicit-fixed-point-conversion"
      "-Wimplicit-float-conversion"
      "-Wimplicit-function-declaration"
      "-Wimplicit-int"
      "-Wimplicit-int-conversion"
      "-Wimplicit-int-float-conversion"
      "-Wimplicit-retain-self"
      "-Wimplicitly-unsigned-literal"
      "-Wimport"
      "-Wimport-preprocessor-directive-pedantic"
      "-Winclude-next-absolute-path"
      "-Winclude-next-outside-header"
      "-Wincompatible-exception-spec"
      "-Wincompatible-function-pointer-types"
      "-Wincompatible-function-pointer-types-strict"
      "-Wincompatible-library-redeclaration"
      "-Wincompatible-ms-pragma-section"
      "-Wincompatible-ms-struct"
      "-Wincompatible-pointer-types"
      "-Wincompatible-pointer-types-discards-qualifiers"
      "-Wincompatible-property-type"
      "-Wincompatible-sysroot"
      "-Wincomplete-framework-module-declaration"
      "-Wincomplete-implementation"
      "-Wincomplete-module"
      "-Wincomplete-setjmp-declaration"
      "-Wincomplete-umbrella"
      "-Winconsistent-dllimport"
      "-Winconsistent-missing-destructor-override"
      "-Winconsistent-missing-override"
      "-Wincrement-bool"
      "-WIndependentClass-attribute"
      "-Winitializer-overrides"
      "-Winjected-class-name"
      "-Winline-asm"
      "-Winline-namespace-reopened-noninline"
      "-Winline-new-delete"
      "-Winstantiation-after-specialization"
      "-Wint-conversion"
      "-Wint-conversions"
      "-Wint-to-pointer-cast"
      "-Wint-to-void-pointer-cast"
      "-Winteger-overflow"
      "-Winvalid-command-line-argument"
      "-Winvalid-constexpr"
      "-Winvalid-iboutlet"
      "-Winvalid-initializer-from-system-header"
      "-Winvalid-ios-deployment-target"
      "-Winvalid-no-builtin-names"
      "-Winvalid-noreturn"
      "-Winvalid-or-nonexistent-directory"
      "-Winvalid-partial-specialization"
      "-Winvalid-pp-token"
      "-Winvalid-source-encoding"
      "-Winvalid-static-assert-message"
      "-Winvalid-token-paste"
      "-Winvalid-unevaluated-string"
      "-Wjump-seh-finally"
      "-Wkeyword-compat"
      "-Wkeyword-macro"
      "-Wknr-promoted-parameter"
      "-Wlanguage-extension-token"
      "-Wlarge-by-value-copy"
      "-Wliblto"
      "-Wlinker-warnings"
      "-Wliteral-conversion"
      "-Wliteral-range"
      "-Wlocal-type-template-args"
      "-Wlogical-op-parentheses"
      "-Wloop-analysis"
      "-Wmacro-redefined"
      "-Wmain-return-type"
      "-Wmalformed-warning-check"
      "-Wmany-braces-around-scalar-init"
      "-Wmathematical-notation-identifier-extension"
      "-Wmax-tokens"
      "-Wmax-unsigned-zero"
      "-Wmemsize-comparison"
      "-Wmethod-signatures"
      "-Wmisexpect"
      "-Wmismatched-parameter-types"
      "-Wmismatched-return-types"
      "-Wmissing-constinit"
      "-Wmissing-exception-spec"
      "-Wmissing-method-return-type"
      "-Wmissing-multilib"
      "-Wmissing-noescape"
      "-Wmissing-noreturn"
      "-Wmissing-prototype-for-cc"
      "-Wmissing-selector-name"
      "-Wmissing-sysroot"
      "-Wmisspelled-assumption"
      "-Wmodule-conflict"
      "-Wmodule-file-config-mismatch"
      "-Wmodule-file-extension"
      "-Wmodule-import-in-extern-c"
      "-Wmodules-ambiguous-internal-linkage"
      "-Wmodules-import-nested-redundant"
      "-Wmost"
      "-Wmove"
      "-Wmsvc-include"
      "-Wmsvc-not-found"
      "-Wmulti-gpu"
      "-Wmultichar"
      "-Wmultiple-move-vbase"
      "-Wnarrowing"
      "-Wnested-anon-types"
      "-Wnew-returns-null"
      "-Wnewline-eof"
      "-Wnsconsumed-mismatch"
      "-WNSObject-attribute"
      "-Wnsreturns-mismatch"
      "-Wnull-arithmetic"
      "-Wnull-character"
      "-Wnull-conversion"
      "-Wnull-pointer-arithmetic"
      "-Wnull-pointer-subtraction"
      "-Wnullability"
      "-Wnullability-completeness"
      "-Wnullability-completeness-on-arrays"
      "-Wnullability-declspec"
      "-Wnullability-extension"
      "-Wnullability-inferred-on-nested-type"
      "-Wnullable-to-nonnull-conversion"
      "-Wodr"
      "-Wopencl-unsupported-rgba"
      "-Wopenmp"
      "-Wopenmp-51-extensions"
      "-Wopenmp-clauses"
      "-Wopenmp-extensions"
      "-Wopenmp-loop-form"
      "-Wopenmp-mapping"
      "-Wopenmp-target"
      "-Wopenmp-target-exception"
      "-Woption-ignored"
      "-Wordered-compare-function-pointers"
      "-Wout-of-line-declaration"
      "-Wout-of-scope-function"
      "-Wover-aligned"
      "-Woverflow"
      "-Woverloaded-shift-op-parentheses"
      "-Woverride-init"
      "-Woverride-module"
      "-Woverriding-method-mismatch"
      "-Woverriding-option"
      "-Wpacked-non-pod"
      "-Wparentheses-equality"
      "-Wpartial-availability"
      "-Wpass-failed"
      "-Wpch-date-time"
      "-Wpedantic-core-features"
      "-Wpedantic-macros"
      "-Wpointer-bool-conversion"
      "-Wpointer-compare"
      "-Wpointer-integer-compare"
      "-Wpointer-to-enum-cast"
      "-Wpointer-to-int-cast"
      "-Wpointer-type-mismatch"
      "-Wpoison-system-directories"
      "-Wpotentially-direct-selector"
      "-Wpotentially-evaluated-expression"
      "-Wpragma-clang-attribute"
      "-Wpragma-once-outside-header"
      "-Wpragma-pack"
      "-Wpragma-pack-suspicious-include"
      "-Wpragma-system-header-outside-header"
      "-Wpragmas"
      "-Wpre-c23-compat"
      "-Wpre-c23-compat-pedantic"
      "-Wpre-c2x-compat"
      "-Wpre-c2x-compat-pedantic"
      "-Wpre-openmp-51-compat"
      "-Wpredefined-identifier-outside-function"
      "-Wprivate-extern"
      "-Wprivate-header"
      "-Wprivate-module"
      "-Wprofile-instr-missing"
      "-Wprofile-instr-out-of-date"
      "-Wprofile-instr-unprofiled"
      "-Wproperty-access-dot-syntax"
      "-Wproperty-attribute-mismatch"
      "-Wprotocol-property-synthesis-ambiguity"
      "-Wpsabi"
      "-Wqualified-void-return-type"
      "-Wquoted-include-in-framework-header"
      "-Wrange-loop-analysis"
      "-Wrange-loop-bind-reference"
      "-Wread-modules-implicitly"
      "-Wread-only-types"
      "-Wreadonly-iboutlet-property"
      "-Wreceiver-expr"
      "-Wreceiver-forward-class"
      "-Wredeclared-class-member"
      "-Wredundant-consteval-if"
      "-Wredundant-parens"
      "-Wreinterpret-base-class"
      "-Wreorder-ctor"
      "-Wreorder-init-list"
      "-Wrequires-super-attribute"
      "-Wreserved-id-macro"
      "-Wreserved-identifier"
      "-Wreserved-macro-identifier"
      "-Wreserved-module-identifier"
      "-Wreserved-user-defined-literal"
      "-Wrestrict-expansion"
      "-Wretained-language-linkage"
      "-Wreturn-local-addr"
      "-Wreturn-stack-address"
      "-Wreturn-std-move"
      "-Wreturn-type-c-linkage"
      "-Wrewrite-not-bool"
      "-Wrtti"
      "-Wsarif-format-unstable"
      "-Wsection"
      "-Wselector-type-mismatch"
      "-Wself-assign"
      "-Wself-assign-field"
      "-Wself-assign-overloaded"
      "-Wself-move"
      "-Wsemicolon-before-method-body"
      "-Wsentinel"
      "-Wserialized-diagnostics"
      "-Wshadow-all"
      "-Wshadow-field"
      "-Wshadow-field-in-constructor"
      "-Wshadow-field-in-constructor-modified"
      "-Wshadow-uncaptured-local"
      "-Wshift-count-negative"
      "-Wshift-count-overflow"
      "-Wshift-op-parentheses"
      "-Wshift-overflow"
      "-Wshift-sign-overflow"
      "-Wshorten-64-to-32"
      "-Wsigned-enum-bitfield"
      "-Wsigned-unsigned-wchar"
      "-Wsingle-bit-bitfield-constant-conversion"
      "-Wsizeof-array-argument"
      "-Wsizeof-array-decay"
      "-Wslash-u-filename"
      "-Wslh-asm-goto"
      "-Wsometimes-uninitialized"
      "-Wsource-mgr"
      "-Wsource-uses-openmp"
      "-Wspir-compat"
      "-Wspirv-compat"
      "-Wstack-exhausted"
      "-Wstatic-float-init"
      "-Wstatic-in-inline"
      "-Wstatic-inline-explicit-instantiation"
      "-Wstatic-local-in-inline"
      "-Wstatic-self-init"
      "-Wstdlibcxx-not-found"
      "-Wstrict-potentially-direct-selector"
      "-Wstring-concatenation"
      "-Wstring-conversion"
      "-Wstring-plus-char"
      "-Wstring-plus-int"
      "-Wstrlcpy-strlcat-size"
      "-Wstrncat-size"
      "-Wsuggest-attribute=const"
      "-Wsuggest-attribute=malloc"
      "-Wsuggest-attribute=noreturn"
      "-Wsuggest-attribute=pure"
      "-Wsuggest-destructor-override"
      "-Wsuper-class-method-mismatch"
      "-Wsuspicious-bzero"
      "-Wsuspicious-memaccess"
      "-Wswift-name-attribute"
      "-Wswitch-bool"
      "-Wsync-alignment"
      "-Wsync-fetch-and-nand-semantics-changed"
      "-Wtarget-clones-mixed-specifiers"
      "-Wtautological-bitwise-compare"
      "-Wtautological-constant-compare"
      "-Wtautological-constant-in-range-compare"
      "-Wtautological-constant-out-of-range-compare"
      "-Wtautological-negation-compare"
      "-Wtautological-overlap-compare"
      "-Wtautological-pointer-compare"
      "-Wtautological-type-limit-compare"
      "-Wtautological-undefined-compare"
      "-Wtautological-unsigned-char-zero-compare"
      "-Wtautological-unsigned-enum-zero-compare"
      "-Wtautological-unsigned-zero-compare"
      "-Wtautological-value-range-compare"
      "-Wtcb-enforcement"
      "-Wtentative-definition-incomplete-type"
      "-Wthread-safety"
      "-Wthread-safety-analysis"
      "-Wthread-safety-attributes"
      "-Wthread-safety-beta"
      "-Wthread-safety-negative"
      "-Wthread-safety-precise"
      "-Wthread-safety-reference"
      "-Wthread-safety-verbose"
      "-Wtype-safety"
      "-Wtypedef-redefinition"
      "-Wtypename-missing"
      "-Wunable-to-open-stats-file"
      "-Wunaligned-access"
      "-Wunaligned-qualifier-implicit-cast"
      "-Wunavailable-declarations"
      "-Wundef-prefix"
      "-Wundefined-bool-conversion"
      "-Wundefined-func-template"
      "-Wundefined-inline"
      "-Wundefined-internal"
      "-Wundefined-internal-type"
      "-Wundefined-reinterpret-cast"
      "-Wundefined-var-template"
      "-Wunderaligned-exception-object"
      "-Wunevaluated-expression"
      "-Wunguarded-availability"
      "-Wunguarded-availability-new"
      "-Wunicode"
      "-Wunicode-homoglyph"
      "-Wunicode-whitespace"
      "-Wunicode-zero-width"
      "-Wuninitialized-const-reference"
      "-Wunknown-argument"
      "-Wunknown-assumption"
      "-Wunknown-attributes"
      "-Wunknown-cuda-version"
      "-Wunknown-directives"
      "-Wunknown-escape-sequence"
      "-Wunknown-sanitizers"
      "-Wno-unknown-warning-option"
      "-Wunnamed-type-template-args"
      "-Wunneeded-internal-declaration"
      "-Wunneeded-member-function"
      "-Wunqualified-std-cast-call"
      "-Wunreachable-code"
      "-Wunreachable-code-aggressive"
      "-Wunreachable-code-break"
      "-Wunreachable-code-fallthrough"
      "-Wunreachable-code-generic-assoc"
      "-Wunreachable-code-loop-increment"
      "-Wunreachable-code-return"
#      "-Wunsafe-buffer-usage"
      "-Wunsequenced"
      "-Wunsupported-abi"
      "-Wunsupported-abs"
      "-Wunsupported-availability-guard"
      "-Wunsupported-cb"
      "-Wunsupported-dll-base-class-template"
      "-Wunsupported-floating-point-opt"
      "-Wunsupported-friend"
      "-Wunsupported-gpopt"
      "-Wunsupported-nan"
      "-Wunsupported-target-opt"
      "-Wunsupported-visibility"
      "-Wunusable-partial-specialization"
      "-Wunused-argument"
      "-Wunused-command-line-argument"
      "-Wunused-comparison"
      "-Wunused-exception-parameter"
      "-Wunused-getter-return-value"
      "-Wunused-lambda-capture"
      "-Wunused-local-typedef"
      "-Wunused-member-function"
      "-Wunused-private-field"
      "-Wunused-property-ivar"
      "-Wunused-result"
      "-Wunused-template"
      "-Wunused-volatile-lvalue"
      "-Wused-but-marked-unused"
      "-Wuser-defined-literals"
      "-Wuser-defined-warnings"
      "-Wvarargs"
      "-Wvec-elem-size"
      "-Wvector-conversion"
      "-Wvector-conversions"
      "-Wvisibility"
      "-Wvla-extension"
      "-Wvoid-pointer-to-enum-cast"
      "-Wvoid-pointer-to-int-cast"
      "-Wvoid-ptr-dereference"
      "-Wvolatile-register-var"
      "-Wwasm-exception-spec"
      "-Wweak-template-vtables"
      "-Wweak-vtables"
      "-Wwritable-strings"
      "-Wzero-length-array"
      "-Wc++-compat"
    )

    SANITIZER_FLAGS=(
        "-fsanitize=address"
        "-fsanitize=pointer-compare"
        "-fsanitize=pointer-subtract"
        "-fsanitize=leak"
        "-fsanitize=undefined"
        "-fsanitize=shift"
        "-fsanitize=shift-exponent"
        "-fsanitize=shift-base"
        "-fsanitize=integer-divide-by-zero"
        "-fsanitize=unreachable"
        "-fsanitize=vla-bound"
        "-fsanitize=null"
        "-fsanitize=signed-integer-overflow"
        "-fsanitize=bounds"
        "-fsanitize=bounds-strict"
        "-fsanitize=alignment"
        "-fsanitize=object-size"
        "-fsanitize=float-divide-by-zero"
        "-fsanitize=float-cast-overflow"
        "-fsanitize=nonnull-attribute"
        "-fsanitize=returns-nonnull-attribute"
        "-fsanitize=bool"
        "-fsanitize=enum"
        "-fsanitize=pointer-overflow"
        "-fsanitize=builtin"
        "-fsanitize-address-use-after-scope"
#        "-fsanitize-coverage=trace-pc"
#        "-fsanitize-coverage=trace-cmp"
        "-fcf-protection=full"
        "-fharden-compares"
        "-fharden-conditional-branches"
        "-fstack-protector-all"
        "-fstack-clash-protection"
#        "-finstrument-functions"
        "-fharden-control-flow-redundancy"
        "-fno-delete-null-pointer-checks"
        "-fno-omit-frame-pointer"
    )

    ANALYZER_FLAGS=(
# this needs to be handled better        "--analyze"
        "--analyzer"
        "-Xanalyzer"
        "-fanalyzer"
        "-fanalyzer-transitivity"
        "-fanalyzer-verbosity=3"
        "-Wno-analyzer-too-complex"
        "-Wno-analyzer-fd-leak"
    )

    DEBUG_FLAGS=(
        "-g3"
        "-ggdb"
        "-fvar-tracking"
        "-fvar-tracking-assignments"
        "-gcolumn-info"
    )

    # Loop through each flag group and check if the flags are supported
    for flag in "${WARNING_FLAGS[@]}"; do
        if [[ "$flag" == "-fsanitize=pointer-compare" || "$flag" == "-fsanitize=pointer-subtract" ]]; then
            # Include -fsanitize=address as a dependency for pointer-compare and pointer-subtract
            if is_flag_supported "$flag -fsanitize=address"; then
                SUPPORTED_WARNING_FLAGS+=("$flag")
            fi
        elif [[ "$flag" == "-fvar-tracking" || "$flag" == "-fvar-tracking-assignments" ]]; then
            # Include --g as a dependency for pfvar-tracking
            if is_flag_supported "$flag --g"; then
                SUPPORTED_WARNING_FLAGS+=("$flag")
            fi
        else
            # For other flags, check support without address
            if is_flag_supported "$flag"; then
                SUPPORTED_WARNING_FLAGS+=("$flag")
            fi
        fi
    done

    # Loop through each flag group and check if the flags are supported
    for flag in "${SANITIZER_FLAGS[@]}"; do
        if [[ "$flag" == "-fsanitize=pointer-compare" || "$flag" == "-fsanitize=pointer-subtract" ]]; then
            # Include -fsanitize=address as a dependency for pointer-compare and pointer-subtract
            if is_flag_supported "$flag -fsanitize=address"; then
                SUPPORTED_SANITIZER_FLAGS+=("$flag")
            fi
        elif [[ "$flag" == "-fvar-tracking" || "$flag" == "-fvar-tracking-assignments" ]]; then
            # Include --g as a dependency for pfvar-trackinge
            if is_flag_supported "$flag --g"; then
                SUPPORTED_SANITIZER_FLAGS+=("$flag")
            fi
        else
            # For other flags, check support without address
            if is_flag_supported "$flag"; then
                SUPPORTED_SANITIZER_FLAGS+=("$flag")
            fi
        fi
    done

    # Loop through each flag group and check if the flags are supported
    for flag in "${ANALYZER_FLAGS[@]}"; do
        if [[ "$flag" == "-fsanitize=pointer-compare" || "$flag" == "-fsanitize=pointer-subtract" ]]; then
            # Include -fsanitize=address as a dependency for pointer-compare and pointer-subtract
            if is_flag_supported "$flag -fsanitize=address"; then
                SUPPORTED_ANALYZER_FLAGS+=("$flag")
            fi
        elif [[ "$flag" == "-fvar-tracking" || "$flag" == "-fvar-tracking-assignments" ]]; then
            # Include --g as a dependency for pfvar-tracking
            if is_flag_supported "$flag --g"; then
                SUPPORTED_ANALYZER_FLAGS+=("$flag")
            fi
        else
            # For other flags, check support without address
            if is_flag_supported "$flag"; then
                SUPPORTED_ANALYZER_FLAGS+=("$flag")
            fi
        fi
    done

    # Loop through each flag group and check if the flags are supported
    for flag in "${DEBUG_FLAGS[@]}"; do
        if [[ "$flag" == "-fsanitize=pointer-compare" || "$flag" == "-fsanitize=pointer-subtract" ]]; then
            # Include -fsanitize=address as a dependency for pointer-compare and pointer-subtract
            if is_flag_supported "$flag -fsanitize=address"; then
                SUPPORTED_DEBUG_FLAGS+=("$flag")
            fi
        elif [[ "$flag" == "-fvar-tracking" || "$flag" == "-fvar-tracking-assignments" ]]; then
            # Include --g as a dependency for pfvar-tracking
            if is_flag_supported "$flag --g"; then
                SUPPORTED_DEBUG_FLAGS+=("$flag")
            fi
        else
            # For other flags, check support without address
            if is_flag_supported "$flag"; then
                SUPPORTED_DEBUG_FLAGS+=("$flag")
            fi
        fi
    done
}

# Function to generate the Makefile in the current directory
generate_makefile() {
  # Determine the library names based on the platform
      # Define LIBRARIES based on the platform
      case "$(uname -s)" in
          Linux) LIBRARIES="" ;;
          Darwin) LIBRARIES="" ;;
          FreeBSD)
              # Check if the directory name starts with "pthread"
              if [[ "$(basename "$(pwd)")" == pthread* ]]; then
                  LIBRARIES="-lpthread"
              else
                  LIBRARIES=""
              fi
              ;;
          *)
              echo "Unsupported platform"
              exit 1
              ;;
      esac

    # Initialize the Makefile
    echo -e ".PHONY: main main-traceable clean lint all\n" > Makefile
    echo "CC=$CC" >> Makefile
    echo "COMPILATION_FLAGS=-std=c18 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE -D_GNU_SOURCE -D__BSD_VISIBLE -Werror" >> Makefile
    echo "SUPPORTED_WARNING_FLAGS=${SUPPORTED_WARNING_FLAGS[@]}" >> Makefile
    echo "SUPPORTED_SANITIZER_FLAGS=${SUPPORTED_SANITIZER_FLAGS[@]}" >> Makefile
    echo "SUPPORTED_ANALYZER_FLAGS=${SUPPORTED_ANALYZER_FLAGS[@]}" >> Makefile
    echo "SUPPORTED_DEBUG_FLAGS=${SUPPORTED_DEBUG_FLAGS[@]}" >> Makefile
    echo "CLANG_TIDY_CHECKS=-checks=*,-llvmlibc-restrict-system-libc-headers,-altera-struct-pack-align,-readability-identifier-length,-altera-unroll-loops,-cppcoreguidelines-init-variables,-cert-err33-c,-modernize-macro-to-enum,-bugprone-easily-swappable-parameters,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-altera-id-dependent-backward-branch,-concurrency-mt-unsafe,-misc-unused-parameters,-hicpp-signed-bitwise,-google-readability-todo,-cert-msc30-c,-cert-msc50-cpp,-readability-function-cognitive-complexity,-clang-analyzer-security.insecureAPI.strcpy,-cert-env33-c,-android-cloexec-accept,-clang-analyzer-security.insecureAPI.rand,-misc-include-cleaner" >> Makefile
    echo "LIBRARIES=${LIBRARIES}" >> Makefile
    echo "PROGRAMS=" >> Makefile
    echo "LIBS=" >> Makefile
    echo -e "SOURCES=\n" >> Makefile

    # Loop through each .c file in the current directory
    for file in *.c; do
        if [[ -f "$file" ]]; then
            # Extract the file name without extension
            filename="${file%.c}"
            full_path=$(realpath "$file")
            last_dir=$(basename "$(dirname "$full_path")")
            second_to_last_dir=$(basename "$(dirname "$(dirname "$full_path")")")

            # Check if the file is one of the testlib files
            if [[ "$filename" == "testlib-1" || "$filename" == "testlib-2" ]]; then
                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) -shared -fPIC -o lib$filename$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo "LIBS += lib$filename$SHARED_EXT" >> Makefile

                # Generate a shared library rule with the appropriate extension
                echo -e "lib$filename-traceaable$SHARED_EXT: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS)  -shared -fPIC -o lib$filename-traceaable$SHARED_EXT $file \$(LIBRARIES)" >> Makefile
                echo "LIBS += lib$filename-traceaable$SHARED_EXT" >> Makefile
            else
                if [[ "$CC" == "gcc"* ]]; then
                    if [[ "$second_to_last_dir/$last_dir/$file" == "memory/malloc-free/main.c" || "$second_to_last_dir/$last_dir/$file" == "memory/memset/main.c" ]]; then
                        # Add the additional flag for files in the specified directories
                        echo -e COMPILATION_FLAGS+="-Wno-analyzer-use-of-uninitialized-value -Wno-sometimes-uninitialized" >> Makefile
                    fi
                fi

                echo "SOURCES += $filename.c" >> Makefile

                # Generate an executable rule with the supported warning flags
                echo "$filename: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_SANITIZER_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS)  -o $filename $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename\n" >> Makefile

                # Generate a traceable version rule
                echo "$filename-traceable: $file" >> Makefile
                echo -e "\t@\$(CC) \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_ANALYZER_FLAGS) \$(SUPPORTED_DEBUG_FLAGS)  -o $filename-traceable $file \$(LIBRARIES)" >> Makefile
                echo -e "PROGRAMS += $filename-traceable\n" >> Makefile
            fi
        fi
    done

    # Add a clean rule to remove all generated binaries and libraries
    echo -e "clean:" >> Makefile
    echo -e "\t@rm -f \$(PROGRAMS) \$(LIBS)" >> Makefile

    # Add a lint rule to remove all generated binaries and libraries
    echo -e "\nlint:" >> Makefile
	  echo -e "\t@\$(CLANGTIDY) \$(SOURCES) -quiet --warnings-as-errors='*' \$(CLANG_TIDY_CHECKS) -- \$(COMPILATION_FLAGS) \$(CFLAGS) \$(SUPPORTED_WARNING_FLAGS) \$(SUPPORTED_DEBUG_FLAGS) \$(LIBRARIES)" >> Makefile

    # Add an "all" rule to build all programs and libraries
    echo -e "\nall: \$(PROGRAMS) \$(LIBS) lint" >> Makefile

    # Print a message indicating the Makefile generation is complete
    echo -e "\nMakefile generated successfully in directory: $(pwd)"
}

# Function to process directories recursively
process_directories() {
    local dir="$1"

    # Loop through each subdirectory
    for subdir in "$dir"/*/; do
        if [[ -d "$subdir" ]]; then
            # Check if there are .c files in the subdirectory
            if [[ -n $(find "$subdir" -maxdepth 1 -name "*.c" -print -quit) ]]; then
                # Generate Makefile in the subdirectory
                (cd "$subdir" && generate_makefile)
            fi

            # Recursively process subdirectories
            process_directories "$subdir"
        fi
    done
}

# Main script execution starts here

# Echo the value of the CC variable
echo "CC is set to: $CC"

# Determine the shared library extension based on the platform
get_shared_lib_extension

# Call the function to populate the supported flags
populate_supported_flags

# Start processing directories from the current directory
process_directories "."

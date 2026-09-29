#!/bin/bash
# Rebuilds ALL of KLEE from the fork: the POSIX runtime and the core C++.
#
# Needed whenever the core changes, because klee_is_sat and klee_is_certain
# are implemented in SpecialFunctionHandler and are compiled into the binary,
# not into the runtime bitcode. Takes minutes. For POSIX-only edits use
# rebuild_posix.sh, which takes seconds.

set -e

FORK=${FORK:-$HOME/klee_fork}
SRC=${SRC:-/tmp/klee_src}
BUILD=${BUILD:-/tmp/klee_build130stp_z3}

GREEN='\033[1;32m'; DIM='\033[2m'; RESET='\033[0m'

echo -e "${DIM}[1/3] Copying sources from $FORK ...${RESET}"
# POSIX runtime (the file-system API backend)
cp "$FORK/runtime/POSIX/fd.c"           "$SRC/runtime/POSIX/fd.c"
cp "$FORK/runtime/POSIX/fd.h"           "$SRC/runtime/POSIX/fd.h"
cp "$FORK/runtime/POSIX/fd_init.c"      "$SRC/runtime/POSIX/fd_init.c"
cp "$FORK/runtime/POSIX/file_api.c"     "$SRC/runtime/POSIX/file_api.c"
cp "$FORK/runtime/POSIX/CMakeLists.txt" "$SRC/runtime/POSIX/CMakeLists.txt"
cp "$FORK/include/klee/file_api.h"      "$SRC/include/klee/file_api.h"
# Core: the klee_is_sat / klee_is_certain intrinsics
cp "$FORK/include/klee/klee.h"                 "$SRC/include/klee/klee.h"
cp "$FORK/lib/Core/SpecialFunctionHandler.cpp" "$SRC/lib/Core/SpecialFunctionHandler.cpp"
cp "$FORK/lib/Core/SpecialFunctionHandler.h"   "$SRC/lib/Core/SpecialFunctionHandler.h"

echo -e "${DIM}[2/3] Dropping cached runtime bitcode ...${RESET}"
rm -f  "$BUILD"/runtime/lib/libkleeRuntimePOSIX64_*
rm -rf "$BUILD"/runtime/POSIX/CMakeFiles

echo -e "${DIM}[3/3] Rebuilding core and runtime ...${RESET}"
cd "$BUILD"
make -j"$(nproc)"

echo -e "${GREEN}KLEE rebuilt.${RESET}"

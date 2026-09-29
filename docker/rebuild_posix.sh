#!/bin/bash
# Rebuilds ONLY KLEE's POSIX runtime from the fork.
#
# Use this after editing anything under klee_fork/runtime/POSIX or the API
# header. It takes seconds, because the klee binary itself is not relinked.
# If you touched the core C++ (SpecialFunctionHandler, klee.h), use
# rebuild_all.sh instead, since those are compiled into the binary.

set -e

FORK=${FORK:-$HOME/klee_fork}
SRC=${SRC:-/tmp/klee_src}
BUILD=${BUILD:-/tmp/klee_build130stp_z3}

GREEN='\033[1;32m'; DIM='\033[2m'; RESET='\033[0m'

echo -e "${DIM}[1/3] Copying POSIX runtime sources from $FORK ...${RESET}"
cp "$FORK/runtime/POSIX/fd.c"           "$SRC/runtime/POSIX/fd.c"
cp "$FORK/runtime/POSIX/fd.h"           "$SRC/runtime/POSIX/fd.h"
cp "$FORK/runtime/POSIX/fd_init.c"      "$SRC/runtime/POSIX/fd_init.c"
cp "$FORK/runtime/POSIX/file_api.c"     "$SRC/runtime/POSIX/file_api.c"
cp "$FORK/runtime/POSIX/CMakeLists.txt" "$SRC/runtime/POSIX/CMakeLists.txt"
cp "$FORK/include/klee/file_api.h"      "$SRC/include/klee/file_api.h"

echo -e "${DIM}[2/3] Dropping cached runtime bitcode ...${RESET}"
rm -f  "$BUILD"/runtime/lib/libkleeRuntimePOSIX64_*
rm -rf "$BUILD"/runtime/POSIX/CMakeFiles

echo -e "${DIM}[3/3] Rebuilding the POSIX runtime ...${RESET}"
cd "$BUILD"
make -j"$(nproc)" RuntimePOSIX

echo -e "${GREEN}POSIX runtime rebuilt.${RESET}"

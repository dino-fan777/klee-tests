/*
 * test_03.c - close invalid fd (-1) fails (EBADF)
 *
 * Pure close test - no open, no file setup.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   assert_close_fails(-1);

   return 0;
}
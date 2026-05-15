/*
 * test_11.c - Seek on invalid fd (-1) fails (EBADF)
 *
 * Only pure lseek test - no open, no file setup.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   assert_lseek_fails(-1, 0, SEEK_SET);

   return 0;
}
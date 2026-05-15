/*
 * test_07.c - write to invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   assert_write_fails(-1, "hello", 5);

   return 0;
}
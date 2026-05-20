/*
 * test_04.c - close invalid fd (99) fails (EBADF)
 *
 * fd way out of range (KLEE max is 32 due to size of the fd array).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   assert_close_fails(99);

   return 0;
}
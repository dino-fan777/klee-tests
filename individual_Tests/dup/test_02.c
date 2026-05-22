/*
 * test_02.c - dup on invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   assert_dup_fails(-1);

   return 0;
}
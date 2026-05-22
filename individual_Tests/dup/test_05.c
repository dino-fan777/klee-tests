/*
 * test_05.c - dup2 with invalid oldfd fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   assert_dup2_fails(-1, 10);

   return 0;
}
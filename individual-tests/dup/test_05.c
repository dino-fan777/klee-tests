/*
 * test_05.c - dup2 with invalid oldfd fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc
 */
#include "test_helper.h"

int main(void) {
   cleanup_fd(__file_create("A_data"));
   int ret = dup2(-1, 10);
   __gen_assert(dup2_fails(ret));

   return 0;
}
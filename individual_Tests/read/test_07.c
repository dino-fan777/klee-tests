/*
 * test_07.c - read from invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   char buf[5] = {0};
   assert_read_fails(-1, buf, 5);

   return 0;
}
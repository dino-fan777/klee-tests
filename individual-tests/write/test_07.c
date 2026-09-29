/*
 * test_07.c - write to invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc
 */
#include "test_helper.h"

int main(void) {
   cleanup_fd(__file_create("A_data"));
   ssize_t wret = write(-1, "hello", 5);
   __gen_assert(write_error(wret));

   return 0;
}
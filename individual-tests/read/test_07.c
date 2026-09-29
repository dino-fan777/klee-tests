/*
 * test_07.c - read from invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   cleanup_fd(__file_create("A_data"));
   char buf[5] = {0};
   ssize_t rret = read(-1, buf, 5);
   __gen_assert(read_error(rret));

   return 0;
}
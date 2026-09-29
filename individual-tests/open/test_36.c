/*
 * test_36.c - Double open O_RDWR + O_RDWR (no close)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_36.c
 * Run    : klee --posix-runtime --libc=uclibc test_36.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];

   declare_symbolic_file_name(fname);
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int fd1 = open(fname, O_RDWR);
   __gen_assert(open_succeeds(fd1));

   int fd2 = open(fname, O_RDWR);
   __gen_assert(open_succeeds(fd2));

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
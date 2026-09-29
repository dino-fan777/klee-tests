/*
 * test_32.c - Double open O_RDONLY + O_RDONLY (no close in between)
 *
 * Second open() on same sym-file fails because first open overwrites st_mode
 * creating a mixed symbolic/concrete expression the solver can't handle.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_32.c
 * Run    : klee --posix-runtime --libc=uclibc test_32.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];

   declare_symbolic_file_name(fname);
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int fd1 = open(fname, O_RDONLY);
   __gen_assert(open_succeeds(fd1));

   int fd2 = open(fname, O_RDONLY);
   __gen_assert(open_succeeds(fd2));

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
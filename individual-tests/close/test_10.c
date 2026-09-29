/*
 * test_10.c - close does not affect other open fds
 *
 * Opens file twice (two fds to same file), closes first fd,
 * verifies second fd still works by reading from it.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd1 = open(fname, flags, 0);
   __gen_assert(open_succeeds(fd1));

   int fd2 = open(fname, O_RDONLY, 0);
   __gen_assert(open_succeeds(fd2));

   int cret = close(fd1);
   __gen_assert(close_succeeds(cret));

   //fd2 should still work
   char buf[5] = {0};
   ssize_t rret = read(fd2, buf, 5);
   __gen_assert(read_all(rret, 5));

   cleanup_fd(fd2);
   return 0;
}
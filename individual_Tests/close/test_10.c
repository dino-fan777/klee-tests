/*
 * test_10.c - close does not affect other open fds
 *
 * Opens file twice (two fds to same file), closes first fd,
 * verifies second fd still works by reading from it.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd1 = open(fname, flags, 0);
   assert_open_succeeds(fd1);

   int fd2 = open(fname, O_RDONLY, 0);
   assert_open_succeeds(fd2);

   assert_close_succeeds(fd1);

   //fd2 should still work
   char buf[5] = {0};
   assert_read_succeeds(fd2, buf, 5);

   cleanup_fd(fd2);
   return 0;
}
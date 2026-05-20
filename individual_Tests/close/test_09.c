/*
 * test_09.c - close then reopen reuses fd number
 *
 * POSIX reuses the lowest available fd. Close and reopen should get same number.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd1 = open(fname, flags, 0644);
   assert_open_succeeds(fd1);

   assert_close_succeeds(fd1);

   int fd2 = open(fname, flags, 0644);
   assert_open_succeeds(fd2);

   assert_fd_equals(fd1, fd2);

   cleanup_fd(fd2);
   return 0;
}
/*
 * test_51.c - Open + dup, close first, reopen fills the gap
 *
 * fd1=3, fd2=4 (via dup). Close fd1. Reopen should get fd=3 (the gap).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_51.c
 * Run    : klee --posix-runtime --libc=uclibc test_51.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd1 = open(fname, flags);
   assert_open_succeeds(fd1);

   int fd2 = assert_dup_succeeds(fd1);

   //close fd1 (fd=3), fd2 (fd=4) stays open
   assert_close_succeeds(fd1);

   //reopen should fill the gap at fd=3
   int fd3 = open(fname, flags);
   assert_open_succeeds(fd3);
   assert_fd_equals(fd1, fd3);

   cleanup_fd(fd2);
   cleanup_fd(fd3);
   return 0;
}
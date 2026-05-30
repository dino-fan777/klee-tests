/*
 * test_14.c - Second fd is fd=4 (via dup)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_14.c
 * Run    : klee --posix-runtime --libc=uclibc test_14.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd1 = open(fname, flags);

   assert_open_succeeds(fd1);
   assert_fd_equals(fd1, 3);

   int fd2 = assert_dup_succeeds(fd1);

   assert_fd_equals(fd2, 4);

   assert_close_succeeds(fd2);
   assert_close_succeeds(fd1);

   return 0;
}
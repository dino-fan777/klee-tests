/*
 * test_32.c - Double open O_RDONLY + O_RDONLY (no close)
 *
 * Second open() on same sym-file fails because first open overwrites st_mode
 * creating a mixed symbolic/concrete expression the solver can't handle.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_32.c
 * Run    : klee --posix-runtime --libc=uclibc test_32.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   assume_file_exists();

   int fd1 = open(fname, O_RDONLY);
   assert_open_succeeds(fd1);

   int fd2 = open(fname, O_RDONLY);
   assert_open_succeeds(fd2);

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
/*
 * test_06.c - dup2 with newfd >= MAX_FDS fails (EBADF)
 *
 * MAX_FDS is 32 in KLEE. Passing newfd=32 is out of range.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_dup2_fails(fd, 32);

   cleanup_fd(fd);
   return 0;
}
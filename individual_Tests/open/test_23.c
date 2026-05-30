/*
 * test_23.c - Open O_RDONLY, close, reopen O_RDONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_23.c
 * Run    : klee --posix-runtime --libc=uclibc test_23.bc --sym-files 1 10
 */
#include "klee_test_helper.h"
 
int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDONLY);
 
   int fd1 = open(fname, flags);
   assert_open_succeeds(fd1);
   assert_close_succeeds(fd1);
 
   int fd2 = open(fname, flags);
   assert_open_succeeds(fd2);
   assert_fd_equals(fd1, fd2);
 
   cleanup_fd(fd2);
   return 0;
}
 

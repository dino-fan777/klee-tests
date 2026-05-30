/*
 * test_31.c - Open O_RDWR, close, reopen O_WRONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_31.c
 * Run    : klee --posix-runtime --libc=uclibc test_31.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   assume_file_exists();

   int fd1 = open(fname, O_RDWR);
   assert_open_succeeds(fd1);
   assert_close_succeeds(fd1);

   int fd2 = open(fname, O_WRONLY);
   assert_open_succeeds(fd2);

   cleanup_fd(fd2);
   return 0;
}
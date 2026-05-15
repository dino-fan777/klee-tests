/*
 * test_02.c - read 5 bytes from O_RDWR file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char buf[5] = {0};
   assert_read_succeeds(fd, buf, 5);

   cleanup_fd(fd);
   return 0;
}
/*
 * test_12.c - lseek to middle then read
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR | O_TRUNC);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "abcdef", 6);
   lseek(fd, 3, SEEK_SET);

   char buf[3] = {0};
   assert_read_succeeds(fd, buf, 3);
   assert_buffers_match("def", buf, 3);

   cleanup_fd(fd);
   return 0;
}
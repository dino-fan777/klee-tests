/*
 * test_14.c - O_TRUNC: SEEK_SET after write, read back and verify
 *
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_14.c
 * Run    : klee --posix-runtime --libc=uclibc test_14.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR | O_TRUNC);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "hello", 5);

   assert_lseek(fd, 0, SEEK_SET, 0);

   char buf[5] = {0};
   assert_read_succeeds(fd, buf, 5);
   assert_buffers_match("hello", buf, 5);

   cleanup_fd(fd);
   return 0;
}
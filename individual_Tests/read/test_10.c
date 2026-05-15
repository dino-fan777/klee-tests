/*
 * test_10.c - write then read back, compare buffers
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR | O_TRUNC);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char wbuf[] = "hello";
   assert_write_succeeds(fd, wbuf, 5);

   lseek(fd, 0, SEEK_SET);

   char rbuf[5] = {0};
   assert_read_succeeds(fd, rbuf, 5);
   assert_buffers_match(wbuf, rbuf, 5);

   cleanup_fd(fd);
   return 0;
}
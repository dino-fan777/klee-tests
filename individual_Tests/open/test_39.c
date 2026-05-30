/*
 * test_39.c - O_RDWR sets both, read and write succeed
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_39.c
 * Run    : klee --posix-runtime --libc=uclibc test_39.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDWR);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   lseek(fd, 0, SEEK_SET);
   assert_write_succeeds(fd, "hello", 5);

   lseek(fd, 0, SEEK_SET);
   char buf[5] = {0};
   assert_read_succeeds(fd, buf, 5);
   assert_buffers_match("hello", buf, 5);

   cleanup_fd(fd);
   return 0;
}
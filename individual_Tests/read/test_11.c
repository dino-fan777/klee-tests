/*
 * test_11.c - two sequential reads cover full content
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
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
   lseek(fd, 0, SEEK_SET);

   char r1[3] = {0};
   assert_read_succeeds(fd, r1, 3);
   assert_buffers_match("abc", r1, 3);

   char r2[3] = {0};
   assert_read_succeeds(fd, r2, 3);
   assert_buffers_match("def", r2, 3);

   cleanup_fd(fd);
   return 0;
}
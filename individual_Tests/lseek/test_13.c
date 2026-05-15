/*
 * test_13.c - Write then seek to multiple positions, read and verify data
 *
 * Opens O_RDWR (no O_TRUNC), overwrites first 6 bytes with 'abcdef',
 * then uses assert_seek_and_read to verify bytes at positions 0, 2, 5.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   lseek(fd, 0, SEEK_SET);
   assert_write_succeeds(fd, "abcdef", 6);

   assert_seek_and_read(fd, 0, 'a');
   assert_seek_and_read(fd, 2, 'c');
   assert_seek_and_read(fd, 5, 'f');

   cleanup_fd(fd);
   return 0;
}
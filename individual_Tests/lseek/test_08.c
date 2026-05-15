/*
 * test_08.c - SEEK_END with negative offset (back from end)
 *
 * 10-byte file, lseek(-3, SEEK_END) should return 7.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_lseek(fd, -3, SEEK_END, 7);

   cleanup_fd(fd);
   return 0;
}
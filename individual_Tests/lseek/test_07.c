/*
 * test_07.c - SEEK_END with 0 (get file size)
 *
 * 10-byte sym-file, lseek(0, SEEK_END) should return 10.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_lseek(fd, 0, SEEK_END, 10);

   cleanup_fd(fd);
   return 0;
}
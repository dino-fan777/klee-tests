/*
 * test_09.c - Seek past EOF (allowed on regular files)
 *
 * 10-byte file, lseek(100, SEEK_SET) should return 100.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_lseek(fd, 100, SEEK_SET, 100);

   cleanup_fd(fd);
   return 0;
}
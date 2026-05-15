/*
 * test_03.c - SEEK_SET to end of 10-byte file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   //10 at the moment since our test files have 10 bytes size
   assert_lseek(fd, 10, SEEK_SET, 10);

   cleanup_fd(fd);
   return 0;
}
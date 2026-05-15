/*
 * test_06.c - SEEK_CUR with 0 (get current position without moving)
 *
 * Seeks to 7, then lseek(0, SEEK_CUR) should return 7.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   lseek(fd, 7, SEEK_SET);

   assert_lseek(fd, 0, SEEK_CUR, 7);

   cleanup_fd(fd);
   return 0;
}
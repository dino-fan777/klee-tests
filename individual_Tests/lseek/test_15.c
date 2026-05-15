/*
 * test_15.c - O_TRUNC: SEEK_END verifies truncated size after write
 *
 * NOTE: O_TRUNC has known issues in KLEE - SEEK_END may report the original
 * buffer capacity (10) instead of the written size (4).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_15.c
 * Run    : klee --posix-runtime --libc=uclibc test_15.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR | O_TRUNC);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "test", 4);

   assert_lseek(fd, 0, SEEK_SET, 0);
   assert_lseek(fd, 0, SEEK_END, 4);

   cleanup_fd(fd);
   return 0;
}
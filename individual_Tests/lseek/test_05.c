/*
 * test_05.c - SEEK_CUR backward from current position
 *
 * Seeks to 8, then seeks -3 backward (offset=5).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   lseek(fd, 8, SEEK_SET);

   assert_lseek(fd, -3, SEEK_CUR, 5);

   cleanup_fd(fd);
   return 0;
}
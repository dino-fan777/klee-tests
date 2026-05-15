/*
 * test_04.c - SEEK_CUR forward from current position
 *
 * Reads 3 bytes (offset=3), then seeks +4 forward (offset=7).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char skip[3] = {0};
   assert_read_succeeds(fd, skip, 3);

   assert_lseek(fd, 4, SEEK_CUR, 7);

   cleanup_fd(fd);
   return 0;
}
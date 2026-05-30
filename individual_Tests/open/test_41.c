/*
 * test_41.c - Open O_RDWR, write 5, offset moves to 5
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_41.c
 * Run    : klee --posix-runtime --libc=uclibc test_41.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDWR);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "hello", 5);
   assert_lseek(fd, 0, SEEK_CUR, 5);

   cleanup_fd(fd);
   return 0;
}
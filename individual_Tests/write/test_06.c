/*
 * test_06.c - write to closed fd fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_WRONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   cleanup_fd(fd);

   assert_write_fails(fd, "hello", 5);

   return 0;
}
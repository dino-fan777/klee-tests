/*
 * test_05.c - write 1 byte (minimal write) succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_WRONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "X", 1);

   cleanup_fd(fd);
   return 0;
}
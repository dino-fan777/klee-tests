/*
 * test_17.c - O_TRUNC | O_WRONLY on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_17.c
 * Run    : klee --posix-runtime --libc=uclibc test_17.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_TRUNC | O_WRONLY);

   int fd = open(fname, flags);

   assert_open_succeeds(fd);

   cleanup_fd(fd);
   return 0;
}
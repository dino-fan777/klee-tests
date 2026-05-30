/*
 * test_12.c - O_RDONLY on a write-only permissions file fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);

   assert_open_fails(fd);

   cleanup_fd(fd);
   return 0;
}
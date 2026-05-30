/*
 * test_49.c - Open with symbolic fname constrained to non-existing
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_49.c
 * Run    : klee --posix-runtime --libc=uclibc test_49.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_not_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);
   assert_open_fails(fd);

   cleanup_fd(fd);
   return 0;
}
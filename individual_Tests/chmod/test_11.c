/*
 * test_11.c - chmod 0000 then open O_RDONLY fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0000);
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);
   assert_open_fails(fd);

   cleanup_fd(fd);
   return 0;
}
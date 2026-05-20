/*
 * test_09.c - chmod 0444 then open O_WRONLY fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0444);
   assume_flags(O_WRONLY);

   int fd = open(fname, flags);
   assert_open_fails(fd);

   cleanup_fd(fd);
   return 0;
}
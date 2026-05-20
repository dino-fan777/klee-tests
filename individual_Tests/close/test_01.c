/*
 * test_01.c - close valid O_RDONLY fd succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_01.c
 * Run    : klee --posix-runtime --libc=uclibc test_01.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0);
   assert_open_succeeds(fd);

   assert_close_succeeds(fd);

   return 0;
}
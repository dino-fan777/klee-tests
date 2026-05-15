/*
 * test_02.c - SEEK_SET to middle of file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   //at the moment 5 since msot of our tests have size 10
   assert_lseek(fd, 5, SEEK_SET, 5);

   cleanup_fd(fd);
   return 0;
}
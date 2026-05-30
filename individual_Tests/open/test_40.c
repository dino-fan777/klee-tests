/*
 * test_40.c - Open starts at offset 0
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_40.c
 * Run    : klee --posix-runtime --libc=uclibc test_40.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   assert_lseek(fd, 0, SEEK_CUR, 0);

   cleanup_fd(fd);
   return 0;
}
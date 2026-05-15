/*
 * test_12.c - Seek on closed fd fails (EBADF??)
 *
 * KLEE's POSIX runtime may not enforce closed fd check on lseek().
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   close(fd);

   assert_lseek_fails(fd, 0, SEEK_SET);

   return 0;
}
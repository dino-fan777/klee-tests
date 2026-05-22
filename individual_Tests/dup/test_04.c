/*
 * test_04.c - dup2 to specific target fd succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   int fd2 = assert_dup2_succeeds(fd, 10);

   cleanup_fd(fd);
   cleanup_fd(fd2);
   return 0;
}
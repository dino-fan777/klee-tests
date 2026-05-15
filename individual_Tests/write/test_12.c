/*
 * test_12.c - O_CREAT | O_WRONLY write to new file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_not_exists();
   assume_flags(O_CREAT | O_WRONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   assert_write_succeeds(fd, "new!", 4);

   cleanup_fd(fd);
   return 0;
}
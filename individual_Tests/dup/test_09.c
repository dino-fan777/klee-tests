/*
 * test_09.c - dup2 to already-open fd closes target first
 *
 * Opens two fds. dup2 overwrites second with first. Second fd now
 * points to first's file. Verifies by reading through new fd2.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd1 = open(fname, flags, 0644);
   assert_open_succeeds(fd1);

   int fd2 = open(fname, flags, 0644);
   assert_open_succeeds(fd2);

   //dup2 overwrites fd2 with fd1, closes old fd2 first
   assert_dup2_succeeds(fd1, klee_get_value_i32(fd2));

   //fd2 should still work (now a copy of fd1)
   char buf[5] = {0};
   assert_read_succeeds(fd2, buf, 5);

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
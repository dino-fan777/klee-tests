/*
 * test_10.c - dup'd fd shares same file, write then read through other
 *
 * Write through fd1, seek to 0 on fd2, read through fd2. Data should match
 * because both fds point to the same dfile buffer.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR);

   int fd1 = open(fname, flags, 0644);
   assert_open_succeeds(fd1);

   int fd2 = assert_dup_succeeds(fd1);

   //write through fd1
   lseek(fd1, 0, SEEK_SET);
   assert_write_succeeds(fd1, "hello", 5);

   //read through fd2 — same dfile, different fd
   lseek(fd2, 0, SEEK_SET);
   char buf[5] = {0};
   assert_read_succeeds(fd2, buf, 5);
   assert_buffers_match("hello", buf, 5);

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
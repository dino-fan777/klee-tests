/*
 * test_09.c - write with O_WRONLY, close, reopen O_RDONLY, read back, compare
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_WRONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char wbuf[] = "world";
   assert_write_succeeds(fd, wbuf, 5);
   cleanup_fd(fd);

   //flags still symbolic?
   int fd2 = open(fname, O_RDONLY, 0644);
   if (fd2 < 0) {
      printf("  [debug] reopen failed, errno=%d\n", klee_get_value_i32(errno));
      klee_report_error(__FILE__, __LINE__,
         "reopen with O_RDONLY failed", "test_fail");
   }
   printf("[PASS] reopen with O_RDONLY succeeded (fd2=%d)\n",
         klee_get_value_i32(fd2));

   char rbuf[5] = {0};
   assert_read_succeeds(fd2, rbuf, 5);
   assert_buffers_match(wbuf, rbuf, 5);

   cleanup_fd(fd2);
   return 0;
}
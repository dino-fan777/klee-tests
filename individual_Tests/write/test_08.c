/*
 * test_08.c - write 5 bytes with O_RDWR, seek back, read back, compare
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char wbuf[] = "hello";
   assert_write_succeeds(fd, wbuf, 5);

   off_t pos = lseek(fd, 0, SEEK_SET);
   //temporary until tests for lseek are done
   if (pos != 0) {
      printf("  [debug] lseek returned %d\n", klee_get_value_i32(pos));
      klee_report_error(__FILE__, __LINE__,
            "lseek to start failed", "test_fail");
   }
   printf("[PASS] lseek() to start succeeded\n");

   char rbuf[5] = {0};
   assert_read_succeeds(fd, rbuf, 5);
   assert_buffers_match(wbuf, rbuf, 5);

   cleanup_fd(fd);
   return 0;
}
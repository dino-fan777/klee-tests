/*
 * test_13.c - O_TRUNC clears old content, only new data present
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR | O_TRUNC);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   char wbuf[] = "xyz";
   assert_write_succeeds(fd, wbuf, 3);

   lseek(fd, 0, SEEK_SET);

   char rbuf[3] = {0};
   assert_read_succeeds(fd, rbuf, 3);
   assert_buffers_match(wbuf, rbuf, 3);

   //temporary because of lseek
   char extra[1] = {0};
   ssize_t ret = read(fd, extra, 1);
   if (ret != 0) {
      printf("  [debug] expected EOF (0), got ret=%d\n",
            klee_get_value_i32(ret));
      klee_report_error(__FILE__, __LINE__,
         "O_TRUNC did not clear old content, extra bytes found",
         "test_fail");
   }
   printf("[PASS] O_TRUNC cleared old content, only 3 new bytes present\n");

   cleanup_fd(fd);
   return 0;
}
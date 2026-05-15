/*
 * test_09.c - second read at EOF returns 0
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

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

  
   char buf[20] = {0};
   assert_read_at_most(fd, buf, 20);

    
   char buf2[1] = {0};
   ssize_t ret = read(fd, buf2, 1);
   int c_ret = klee_get_value_i32(ret);

   if (ret != 0) {
       printf("  [debug] expected EOF (0), got ret=%d\n", c_ret);
       klee_report_error(__FILE__, __LINE__,
           "second read should return 0 (EOF)", "test_fail");
   }
   printf("[PASS] second read at EOF returned 0\n");

   cleanup_fd(fd);
   return 0;
}
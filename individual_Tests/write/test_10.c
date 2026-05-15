/*
 * test_10.c - O_APPEND write goes to end of file, verify offset
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_APPEND | O_WRONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   off_t original_size = lseek(fd, 0, SEEK_END);
   printf("[info] original file size = %d\n", klee_get_value_i32(original_size));

   assert_write_succeeds(fd, "abc", 3);

   off_t current = lseek(fd, 0, SEEK_CUR);
   int c_current  = klee_get_value_i32(current);
   int c_expected = klee_get_value_i32(original_size) + 3;

   //temporary until lseek tests are made
   if (c_current != c_expected) {
      printf("  [debug] offset after append: got=%d expected=%d\n",
            c_current, c_expected);
      klee_report_error(__FILE__, __LINE__,
         "O_APPEND did not write at end of file", "test_fail");
   }
   printf("[PASS] offset after append = %d (original %d + 3)\n",
         c_current, klee_get_value_i32(original_size));

   cleanup_fd(fd);
   return 0;
}
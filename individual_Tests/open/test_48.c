/*
 * test_48.c - Open with symbolic fname constrained to existing file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_48.c
 * Run    : klee --posix-runtime --libc=uclibc test_48.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   int c_fname = klee_get_value_i32(fname[0]);
   if (c_fname != 'A') {
      printf("  [debug] fname resolved to '%c', expected 'A'\n", c_fname);
      klee_report_error(__FILE__, __LINE__,
         "fname should resolve to A (only sym-file)", "test_fail");
   }
   printf("symbolic fname resolved to '%c'\n", c_fname);

   cleanup_fd(fd);
   return 0;
}
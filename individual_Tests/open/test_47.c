/*
 * test_47.c - Open fd table full, returns EMFILE?
 *
 * KLEE has MAX_FDS=32. Fds 0,1,2 are stdin/stdout/stderr. fd 3 is our
 * open. dup to fill remaining 28 slots. Next open should fail EMFILE.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_47.c
 * Run    : klee --posix-runtime --libc=uclibc test_47.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   //fill all remaining fd slots with dup 
   int last_dup = -1;
   for (int i = 0; i < 28; i++) {
      last_dup = dup(fd);
      if (last_dup < 0) break;
   }

   //next open should fail EMFILE
   int fd_full = open(fname, O_RDONLY);
   int c_fd = klee_get_value_i32(fd_full);
   int c_errno = klee_get_value_i32(errno);

   if (c_fd != -1) {
      printf("  [debug] expected EMFILE, got fd=%d\n", c_fd);
      klee_report_error(__FILE__, __LINE__,
         "open should fail with EMFILE when fd table is full", "test_fail");
    }
    
   //printf("[PASS] fd table full — open returned -1, errno=%d\n", c_errno);

   //cleanup all fds
   for (int i = 3; i < 32; i++) close(i);
   return 0;
}
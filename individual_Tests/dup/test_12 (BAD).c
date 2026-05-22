/*
 * test_12.c - dup clears eCloseOnExec on new fd
 *
 * Sets close-on-exec on original via fcntl(F_SETFD), dups it,
 * checks new fd has close-on-exec cleared (POSIX requirement,
 * implemented as f2->flags &= ~eCloseOnExec in dup2).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   //set close-on-exec on original
   fcntl(fd, F_SETFD, FD_CLOEXEC);
   int orig_flags = fcntl(fd, F_GETFD);
   if (!(orig_flags & FD_CLOEXEC)) {
       klee_report_error(__FILE__, __LINE__,
           "F_SETFD did not set close-on-exec", "test_fail");
   }
   printf("[PASS] original fd has close-on-exec set\n");

   // dup should clear close-on-exec
   int fd2 = assert_dup_succeeds(fd);

   int new_flags = fcntl(fd2, F_GETFD);
   if (new_flags & FD_CLOEXEC) {
       klee_report_error(__FILE__, __LINE__,
           "dup'd fd should NOT have close-on-exec", "test_fail");
   }
   printf("[PASS] dup'd fd has close-on-exec cleared\n");

   cleanup_fd(fd);
   cleanup_fd(fd2);
   return 0;
}
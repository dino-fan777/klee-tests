/*
 * fail_limit.c - fail KLEE's file descriptor limit
 * 
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone fail_limit_fd.c
 * Run    : klee --posix-runtime --libc=uclibc fail_limit_fd.c --sym-files 1 1
 */
#include "klee_test_helper.h"
 
int main(void) {
   DECLARE_SYMBOLIC_FNAME();
   DECLARE_SYMBOLIC_FLAGS();
 
   ASSUME_FILE_EXISTS(fname);
   ASSUME_FLAGS(flags, O_RDONLY);
 
   for(int i = 0; i < 150; i++){
      int fd = open(fname, flags, 0644);
      ASSERT_OPEN_SUCCEEDS(fd, flags, fname);
      CLEANUP_FD(fd);
   }

   return 0;
}

/*
 * test_45.c - Open with mode=0644 overwrites st_mode (even without O_CREAT)
 *
 * chmod sets 0777, open with mode=0644 clobbers it. stat shows 0644 not 0777.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_45.c
 * Run    : klee --posix-runtime --libc=uclibc test_45.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assert_chmod_succeeds(0777);
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);
   cleanup_fd(fd);

   //check if open clobbered permissions
   assert_perms(0644);
   

   return 0;
}
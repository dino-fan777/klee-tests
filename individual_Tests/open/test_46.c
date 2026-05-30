/*
 * test_46.c - Open with mode=0000 overwrites st_mode to 0000
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_46.c
 * Run    : klee --posix-runtime --libc=uclibc test_46.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assert_chmod_succeeds(0666);
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0);
   assert_open_succeeds(fd);
   cleanup_fd(fd);

   //check if open clobbered permissions 
   assert_perms(0000);
   //printf("[PASS] BUG B05 confirmed: open(mode=0) overwrote chmod(0666)\n");

   return 0;
}
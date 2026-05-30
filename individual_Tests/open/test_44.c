/*
 * test_44.c - chmod(0222) then open O_RDONLY, passes (has_permission)
 *
 * Should fail with EACCES (no read bits). But has_permission never checks
 * read permission for O_RDONLY because flags & O_RDONLY is always 0.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_44.c
 * Run    : klee --posix-runtime --libc=uclibc test_44.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();
   assume_file_exists();
   assert_chmod_succeeds(0222);
   assume_flags(O_RDONLY);

   //This SHOULD fail but PASSES due to has_permission bug
   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   //printf("[PASS] BUG B04 confirmed: O_RDONLY bypasses read permission check\n");

   cleanup_fd(fd);
   return 0;
}
/*
 * test_06.c - chmod 0000 removes all permissions
 *
 * perms_are() is unreliable, so the resulting bits are verified indirectly:
 * after chmod(0000) the owner must be unable to open O_RDONLY or O_WRONLY.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   declare_symbolic_file_name(fname);

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int cret = chmod(fname, 0000);
   __gen_assert(chmod_succeeds(cret));

   int fd_r = open(fname, O_RDONLY);
   __gen_assert(open_fails(fd_r));
   cleanup_fd(fd_r);

   int fd_w = open(fname, O_WRONLY);
   __gen_assert(open_fails(fd_w));
   cleanup_fd(fd_w);

   return 0;
}

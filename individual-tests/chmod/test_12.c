/*
 * test_12.c - chmod twice, second overwrites first
 *
 * perms_are() is unreliable, so the overwrite is verified indirectly: if
 * the first chmod(0777) still applied, O_WRONLY would succeed. The second
 * chmod(0444) must win, so O_WRONLY must fail.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   declare_symbolic_file_name(fname);

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int cret1 = chmod(fname, 0777);
   __gen_assert(chmod_succeeds(cret1));

   int cret2 = chmod(fname, 0444);
   __gen_assert(chmod_succeeds(cret2));

   int fd = open(fname, O_WRONLY);
   __gen_assert(open_fails(fd));
   cleanup_fd(fd);

   return 0;
}

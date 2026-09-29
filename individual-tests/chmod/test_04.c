/*
 * test_04.c - chmod 0777 sets all permission bits
 *
 * perms_are() is unreliable, so the resulting bits are verified indirectly:
 * after chmod(0777) the owner must be able to open O_RDWR.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   declare_symbolic_file_name(fname);

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int cret = chmod(fname, 0777);
   __gen_assert(chmod_succeeds(cret));

   int fd = open(fname, O_RDWR);
   __gen_assert(open_succeeds(fd));
   cleanup_fd(fd);

   return 0;
}

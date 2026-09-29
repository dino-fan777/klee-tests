/*
 * test_07.c - chmod 0755 sets rwxr-xr-x
 *
 * perms_are() is unreliable, so the resulting bits are verified indirectly:
 * after chmod(0755) the owner must be able to open both O_RDONLY and O_WRONLY.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   declare_symbolic_file_name(fname);

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int cret = chmod(fname, 0755);
   __gen_assert(chmod_succeeds(cret));

   int fd_r = open(fname, O_RDONLY);
   __gen_assert(open_succeeds(fd_r));
   cleanup_fd(fd_r);

   int fd_w = open(fname, O_WRONLY);
   __gen_assert(open_succeeds(fd_w));
   cleanup_fd(fd_w);

   return 0;
}

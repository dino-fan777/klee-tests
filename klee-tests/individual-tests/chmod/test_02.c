/*
 * test_02.c - chmod on non-existing file fails (ENOENT)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   declare_symbolic_file_name(fname);

   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   __assume(file_not_exists(fname));

   int cret = chmod(fname, 0644);
   __gen_assert(chmod_fails(cret));

   return 0;
}

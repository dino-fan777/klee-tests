/*
 * test_43.c - chmod(0444) then open O_WRONLY fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_43.c
 * Run    : klee --posix-runtime --libc=uclibc test_43.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;

   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();
   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   int cret = chmod(fname, 0444);
   __gen_assert(chmod_succeeds(cret));
   __assume(flags_equal(flags, O_WRONLY));

   int fd = open(fname, flags);
   __gen_assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}
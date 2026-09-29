/*
 * test_13.c - First open() returns fd=3
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc --sym-files 1 1
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
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags);

   __gen_assert(open_succeeds(fd));
   __gen_assert(fd_is(fd, 3));

   int cret = close(fd);
   __gen_assert(close_succeeds(cret));

   return 0;
}
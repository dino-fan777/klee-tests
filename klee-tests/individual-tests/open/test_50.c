/*
 * test_50.c - Close fd=3, reopen gets fd=3 (recycling)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_50.c
 * Run    : klee --posix-runtime --libc=uclibc test_50.bc --sym-files 1 10
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

   int fd1 = open(fname, flags);
   __gen_assert(open_succeeds(fd1));
   __gen_assert(fd_is(fd1, 3));

   int ret = close(fd1);
   __gen_assert(close_succeeds(ret));

   int fd2 = open(fname, flags);
   __gen_assert(open_succeeds(fd2));
   __gen_assert(fd_is(fd2, 3));

   cleanup_fd(fd2);
   return 0;
}

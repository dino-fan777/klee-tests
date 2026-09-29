/*
 * test_06.c - dup2 with newfd >= MAX_FDS fails (EBADF)
 *
 * MAX_FDS is 32 in KLEE. Passing newfd=32 is out of range.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 10
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

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   int ret = dup2(fd, 32);
   __gen_assert(dup2_fails(ret));

   cleanup_fd(fd);
   return 0;
}
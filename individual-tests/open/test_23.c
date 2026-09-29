/*
 * test_23.c - Open O_RDONLY, close, reopen O_RDONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_23.c
 * Run    : klee --posix-runtime --libc=uclibc test_23.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;

   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd1 = open(fname, flags);
   __gen_assert(open_succeeds(fd1));
   int cret = close(fd1);
   __gen_assert(close_succeeds(cret));

   int fd2 = open(fname, flags);
   __gen_assert(open_succeeds(fd2));
   __gen_assert(fd_is(fd1, fd2));

   cleanup_fd(fd2);
   return 0;
}
 

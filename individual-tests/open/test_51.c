/*
 * test_51.c - Open + dup, close first, reopen fills the gap
 *
 * fd1=3, fd2=4 (via dup). Close fd1. Reopen should get fd=3 (the gap).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_51.c
 * Run    : klee --posix-runtime --libc=uclibc test_51.bc --sym-files 1 10
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

   int fd2 = dup(fd1);
   __gen_assert(dup_is_new(fd2, fd1));

   //close fd1 (fd=3), fd2 (fd=4) stays open
   int cret = close(fd1);
   __gen_assert(close_succeeds(cret));

   //reopen should fill the gap at fd=3
   int fd3 = open(fname, flags);
   __gen_assert(open_succeeds(fd3));
   __gen_assert(fd_is(fd1, fd3));

   cleanup_fd(fd2);
   cleanup_fd(fd3);
   return 0;
}
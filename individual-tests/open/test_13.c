/*
 * test_13.c - First open() returns fd=3
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc
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

   int fd = open(fname, flags);

   __gen_assert(open_succeeds(fd));
   __gen_assert(fd_is(fd, 3));

   int cret = close(fd);
   __gen_assert(close_succeeds(cret));

   return 0;
}
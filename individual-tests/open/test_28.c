/*
 * test_28.c - Open O_WRONLY, close, reopen O_RDONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_28.c
 * Run    : klee --posix-runtime --libc=uclibc test_28.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];

   declare_symbolic_file_name(fname);
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int fd1 = open(fname, O_WRONLY);
   __gen_assert(open_succeeds(fd1));
   int cret = close(fd1);
   __gen_assert(close_succeeds(cret));

   int fd2 = open(fname, O_RDONLY);
   __gen_assert(open_succeeds(fd2));

   cleanup_fd(fd2);
   return 0;
}
/*
 * test_31.c - Open O_RDWR, close, reopen O_WRONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_31.c
 * Run    : klee --posix-runtime --libc=uclibc test_31.bc --sym-files 1 10
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
   __assume(file_exists(fname));

   int fd1 = open(fname, O_RDWR);
   __gen_assert(open_succeeds(fd1));
   int cret = close(fd1);
   __gen_assert(close_succeeds(cret));

   int fd2 = open(fname, O_WRONLY);
   __gen_assert(open_succeeds(fd2));

   cleanup_fd(fd2);
   return 0;
}
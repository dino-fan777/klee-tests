/*
 * test_07.c - close then write fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_WRONLY));

   int fd = open(fname, flags, 0);
   __gen_assert(open_succeeds(fd));

   int cret = close(fd);
   __gen_assert(close_succeeds(cret));

   ssize_t wret = write(fd, "hello", 5);
   __gen_assert(write_error(wret));

   return 0;
}
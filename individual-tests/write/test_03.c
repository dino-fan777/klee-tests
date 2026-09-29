/*
 * test_03.c - write to O_RDONLY file fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc
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

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   ssize_t wret = write(fd, "hello", 5);
   __gen_assert(write_error(wret));

   cleanup_fd(fd);
   return 0;
}
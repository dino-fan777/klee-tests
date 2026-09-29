/*
 * test_08.c - close then lseek fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
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

   int fd = open(fname, flags, 0);
   __gen_assert(open_succeeds(fd));

   int cret = close(fd);
   __gen_assert(close_succeeds(cret));

   off_t pos = lseek(fd, 0, SEEK_SET);
   __gen_assert(lseek_fails(pos));

   return 0;
}
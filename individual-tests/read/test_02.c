/*
 * test_02.c - read 5 bytes from O_RDWR file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDWR));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __gen_assert(read_all(rret, 5));

   cleanup_fd(fd);
   return 0;
}
/*
 * test_09.c - second read at EOF returns 0
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc
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

   char buf[20] = {0};
   ssize_t rret = read(fd, buf, 20);
   __gen_assert(read_at_most(rret));

   char buf2[1] = {0};
   ssize_t ret = read(fd, buf2, 1);
   __gen_assert(read_all(ret, 0));

   cleanup_fd(fd);
   return 0;
}
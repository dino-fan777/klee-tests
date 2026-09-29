/*
 * test_12.c - close original fd, dup'd fd still works
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 10
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

   int fd2 = dup(fd);
   __gen_assert(dup_is_new(fd2, fd));

   int cret = close(fd);
   __gen_assert(close_succeeds(cret));

   char buf[5] = {0};
   ssize_t rret = read(fd2, buf, 5);
   __gen_assert(read_all(rret, 5));

   cleanup_fd(fd2);
   return 0;
}
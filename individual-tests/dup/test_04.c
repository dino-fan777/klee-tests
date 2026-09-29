/*
 * test_04.c - dup2 to specific target fd succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc --sym-files 1 10
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

   int fd2 = dup2(fd, 10);
   __gen_assert(dup2_returns(fd2, 10));

   cleanup_fd(fd);
   cleanup_fd(fd2);
   return 0;
}
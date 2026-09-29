/*
 * test_01.c - dup on valid fd succeeds, returns new fd
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_01.c
 * Run    : klee --posix-runtime --libc=uclibc test_01.bc
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

   cleanup_fd(fd);
   cleanup_fd(fd2);
   return 0;
}
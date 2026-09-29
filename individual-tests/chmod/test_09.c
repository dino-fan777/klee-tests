/*
 * test_09.c - chmod 0444 then open O_WRONLY fails (EACCES)
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

   int cret = chmod(fname, 0444);
   __gen_assert(chmod_succeeds(cret));

   __assume(flags_equal(flags, O_WRONLY));
   int fd = open(fname, flags);
   __gen_assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}

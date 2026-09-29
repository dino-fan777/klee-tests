/*
 * test_01.c - O_RDONLY on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_01.c
 * Run    : klee --posix-runtime --libc=uclibc test_01.bc --sym-files 1 1
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

   int fd = open(fname, flags);

   __gen_assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}
/*
 * test_49.c - Open with symbolic fname constrained to non-existing
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_49.c
 * Run    : klee --posix-runtime --libc=uclibc test_49.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;

   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();
   cleanup_fd(__file_create("A_data"));
   __assume(file_not_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags);
   __gen_assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}
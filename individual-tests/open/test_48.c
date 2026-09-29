/*
 * test_48.c - Open with symbolic fname constrained to existing file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_48.c
 * Run    : klee --posix-runtime --libc=uclibc test_48.bc
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

   __gen_assert(buffers_match(fname, "A", 1));
   debug_name(fname);

   cleanup_fd(fd);
   return 0;
}
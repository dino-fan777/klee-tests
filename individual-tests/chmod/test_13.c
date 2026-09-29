/*
 * test_13.c - chmod before open gives single completed path
 *
 * Without chmod, open() forks on symbolic st_mode. With chmod, st_mode
 * is concrete only 1 completed path, 0 partial paths.
 * The test is: Check KLEE output: "completed paths = 1"
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));

   int cret = chmod(fname, 0666);
   __gen_assert(chmod_succeeds(cret));

   __assume(flags_equal(flags, O_RDWR));
   int fd = open(fname, flags);
   __gen_assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}

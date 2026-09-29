/*
 * test_12.c - O_CREAT | O_WRONLY write to new file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_not_exists(fname));
   __assume(flags_equal(flags, O_CREAT | O_WRONLY));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   ssize_t wret = write(fd, "new!", 4);
   __gen_assert(write_all(wret, 4));

   cleanup_fd(fd);
   return 0;
}
/*
 * test_11.c - two sequential writes, verify total bytes via lseek
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDWR | O_TRUNC));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   ssize_t wret1 = write(fd, "abc", 3);
   __gen_assert(write_all(wret1, 3));
   ssize_t wret2 = write(fd, "def", 3);
   __gen_assert(write_all(wret2, 3));

   off_t size = lseek(fd, 0, SEEK_CUR);
   __gen_assert(lseek_is(size, 6));

   cleanup_fd(fd);
   return 0;
}
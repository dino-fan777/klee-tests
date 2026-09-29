/*
 * test_12.c - lseek to middle then read
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
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDWR | O_TRUNC));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   ssize_t wret = write(fd, "abcdef", 6);
   __gen_assert(write_all(wret, 6));
   lseek(fd, 3, SEEK_SET);

   char buf[3] = {0};
   ssize_t rret = read(fd, buf, 3);
   __gen_assert(read_all(rret, 3));
   __gen_assert(buffers_match("def", buf, 3));

   cleanup_fd(fd);
   return 0;
}
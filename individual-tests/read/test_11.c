/*
 * test_11.c - two sequential reads cover full content
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
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
   lseek(fd, 0, SEEK_SET);

   char r1[3] = {0};
   ssize_t rret1 = read(fd, r1, 3);
   __gen_assert(read_all(rret1, 3));
   __gen_assert(buffers_match("abc", r1, 3));

   char r2[3] = {0};
   ssize_t rret2 = read(fd, r2, 3);
   __gen_assert(read_all(rret2, 3));
   __gen_assert(buffers_match("def", r2, 3));

   cleanup_fd(fd);
   return 0;
}
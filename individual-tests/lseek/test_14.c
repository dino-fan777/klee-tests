/*
 * test_14.c - O_TRUNC: SEEK_SET after write, read back and verify
 *
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_14.c
 * Run    : klee --posix-runtime --libc=uclibc test_14.bc
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

   ssize_t wret = write(fd, "hello", 5);
   __gen_assert(write_all(wret, 5));

   off_t pos = lseek(fd, 0, SEEK_SET);
   __gen_assert(lseek_is(pos, 0));

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __gen_assert(read_all(rret, 5));
   __gen_assert(buffers_match("hello", buf, 5));

   cleanup_fd(fd);
   return 0;
}
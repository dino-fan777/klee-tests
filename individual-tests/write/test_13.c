/*
 * test_13.c - O_TRUNC clears old content, only new data present
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
   __assume(flags_equal(flags, O_RDWR | O_TRUNC));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   char wbuf[] = "xyz";
   ssize_t wret = write(fd, wbuf, 3);
   __gen_assert(write_all(wret, 3));

   lseek(fd, 0, SEEK_SET);

   char rbuf[3] = {0};
   ssize_t rret = read(fd, rbuf, 3);
   __gen_assert(read_all(rret, 3));
   __gen_assert(buffers_match(wbuf, rbuf, 3));

   char extra[1] = {0};
   ssize_t ret = read(fd, extra, 1);
   __gen_assert(read_all(ret, 0));

   cleanup_fd(fd);
   return 0;
}
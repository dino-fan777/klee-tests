/*
 * test_10.c - write then read back, compare buffers
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 10
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

   char wbuf[] = "hello";
   ssize_t wret = write(fd, wbuf, 5);
   __gen_assert(write_all(wret, 5));

   lseek(fd, 0, SEEK_SET);

   char rbuf[5] = {0};
   ssize_t rret = read(fd, rbuf, 5);
   __gen_assert(read_all(rret, 5));
   __gen_assert(buffers_match(wbuf, rbuf, 5));

   cleanup_fd(fd);
   return 0;
}
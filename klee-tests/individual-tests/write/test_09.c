/*
 * test_09.c - write with O_WRONLY, close, reopen O_RDONLY, read back, compare
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc --sym-files 1 1
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_WRONLY));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   char wbuf[] = "world";
   ssize_t wret = write(fd, wbuf, 5);
   __gen_assert(write_all(wret, 5));
   cleanup_fd(fd);

   //flags still symbolic?
   int fd2 = open(fname, O_RDONLY, 0644);
   __gen_assert(open_succeeds(fd2));

   char rbuf[5] = {0};
   ssize_t rret = read(fd2, rbuf, 5);
   __gen_assert(read_all(rret, 5));
   __gen_assert(buffers_match(wbuf, rbuf, 5));

   cleanup_fd(fd2);
   return 0;
}
/*
 * test_13.c - Write then seek to multiple positions, read and verify data
 *
 * Opens O_RDWR (no O_TRUNC), overwrites first 6 bytes with 'abcdef',
 * then seeks and reads 1 byte at a time to verify bytes at positions 0, 2, 5.
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
   __assume(flags_equal(flags, O_RDWR));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   lseek(fd, 0, SEEK_SET);
   ssize_t wret = write(fd, "abcdef", 6);
   __gen_assert(write_all(wret, 6));

   lseek(fd, 0, SEEK_SET);
   char buf0[1] = {0};
   ssize_t rret0 = read(fd, buf0, 1);
   __gen_assert(read_all(rret0, 1));
   __gen_assert(buffers_match("a", buf0, 1));

   lseek(fd, 2, SEEK_SET);
   char buf1[1] = {0};
   ssize_t rret1 = read(fd, buf1, 1);
   __gen_assert(read_all(rret1, 1));
   __gen_assert(buffers_match("c", buf1, 1));

   lseek(fd, 5, SEEK_SET);
   char buf2[1] = {0};
   ssize_t rret2 = read(fd, buf2, 1);
   __gen_assert(read_all(rret2, 1));
   __gen_assert(buffers_match("f", buf2, 1));

   cleanup_fd(fd);
   return 0;
}
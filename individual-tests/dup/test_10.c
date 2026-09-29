/*
 * test_10.c - dup'd fd shares same file, write then read through other
 *
 * Write through fd1, seek to 0 on fd2, read through fd2. Data should match
 * because both fds point to the same dfile buffer.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc
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

   int fd1 = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd1));

   int fd2 = dup(fd1);
   __gen_assert(dup_is_new(fd2, fd1));

   //write through fd1
   lseek(fd1, 0, SEEK_SET);
   ssize_t wret = write(fd1, "hello", 5);
   __gen_assert(write_all(wret, 5));

   //read through fd2 — same dfile, different fd
   lseek(fd2, 0, SEEK_SET);
   char buf[5] = {0};
   ssize_t rret = read(fd2, buf, 5);
   __gen_assert(read_all(rret, 5));
   __gen_assert(buffers_match("hello", buf, 5));

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
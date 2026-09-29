/*
 * test_09.c - dup2 to already-open fd closes target first
 *
 * Opens two fds. dup2 overwrites second with first. Second fd now
 * points to first's file. Verifies by reading through new fd2.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd1 = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd1));

   int fd2 = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd2));

   //dup2 overwrites fd2 with fd1, closes old fd2 first
   int newfd = klee_get_value_i32(fd2);
   int dret = dup2(fd1, newfd);
   __gen_assert(dup2_returns(dret, newfd));

   //fd2 should still work (now a copy of fd1)
   char buf[5] = {0};
   ssize_t rret = read(fd2, buf, 5);
   __gen_assert(read_all(rret, 5));

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
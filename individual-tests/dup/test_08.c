/*
 * test_08.c - dup2(fd, fd) same fd returns fd (no-op)
 *
 * The code does *f2 = *f which is a self-copy. POSIX says dup2 to self
 * should return fd without closing. KLEE does this correctly by accident.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
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

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   int newfd = klee_get_value_i32(fd);
   int ret = dup2(fd, newfd);
   __gen_assert(dup2_returns(ret, newfd));

   cleanup_fd(fd);
   return 0;
}
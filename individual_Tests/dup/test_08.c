/*
 * test_08.c - dup2(fd, fd) same fd returns fd (no-op)
 *
 * The code does *f2 = *f which is a self-copy. POSIX says dup2 to self
 * should return fd without closing. KLEE does this correctly by accident.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDONLY);

   int fd = open(fname, flags, 0644);
   assert_open_succeeds(fd);

   int ret = assert_dup2_succeeds(fd, klee_get_value_i32(fd));

   cleanup_fd(fd);
   return 0;
}
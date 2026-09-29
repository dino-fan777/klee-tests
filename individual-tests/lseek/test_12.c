/*
 * test_12.c - Seek on closed fd fails (EBADF??)
 *
 * KLEE's POSIX runtime may not enforce closed fd check on lseek().
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc --sym-files 1 10
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

   close(fd);

   off_t pos = lseek(fd, 0, SEEK_SET);
   __gen_assert(lseek_fails(pos));

   return 0;
}
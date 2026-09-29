/*
 * test_03.c - SEEK_SET to end of 10-byte file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc --sym-files 1 10
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

   //10 at the moment since our test files have 10 bytes size
   off_t pos = lseek(fd, 10, SEEK_SET);
   __gen_assert(lseek_is(pos, 10));

   cleanup_fd(fd);
   return 0;
}
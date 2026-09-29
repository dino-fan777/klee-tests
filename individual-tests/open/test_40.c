/*
 * test_40.c - Open starts at offset 0
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_40.c
 * Run    : klee --posix-runtime --libc=uclibc test_40.bc --sym-files 1 10
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

   int fd = open(fname, flags);
   __gen_assert(open_succeeds(fd));

   off_t pos = lseek(fd, 0, SEEK_CUR);
   __gen_assert(lseek_is(pos, 0));

   cleanup_fd(fd);
   return 0;
}
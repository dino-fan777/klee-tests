/*
 * test_05.c - SEEK_CUR backward from current position
 *
 * Seeks to 8, then seeks -3 backward (offset=5).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc --sym-files 1 10
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

   lseek(fd, 8, SEEK_SET);

   off_t pos = lseek(fd, -3, SEEK_CUR);
   __gen_assert(lseek_is(pos, 5));

   cleanup_fd(fd);
   return 0;
}
/*
 * test_08.c - SEEK_END with negative offset (back from end)
 *
 * 10-byte file, lseek(-3, SEEK_END) should return 7.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 10
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
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   off_t pos = lseek(fd, -3, SEEK_END);
   __gen_assert(lseek_is(pos, 7));

   cleanup_fd(fd);
   return 0;
}
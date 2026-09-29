/*
 * test_46.c - Open with mode=0000 overwrites st_mode to 0000
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_46.c
 * Run    : klee --posix-runtime --libc=uclibc test_46.bc --sym-files 1 10
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
   int cret = chmod(fname, 0666);
   __gen_assert(chmod_succeeds(cret));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags, 0);
   __gen_assert(open_succeeds(fd));

   //check if open clobbered permissions
   __gen_assert(perms_are(fd, 0000));
   //printf("[PASS] BUG B05 confirmed: open(mode=0) overwrote chmod(0666)\n");

   cleanup_fd(fd);

   return 0;
}
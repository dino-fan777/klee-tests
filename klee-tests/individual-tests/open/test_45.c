/*
 * test_45.c - Open with mode=0644 overwrites st_mode (even without O_CREAT)
 *
 * chmod sets 0777, open with mode=0644 clobbers it. stat shows 0644 not 0777.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_45.c
 * Run    : klee --posix-runtime --libc=uclibc test_45.bc --sym-files 1 10
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
   int cret = chmod(fname, 0777);
   __gen_assert(chmod_succeeds(cret));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   //check if open clobbered permissions
   __gen_assert(perms_are(fd, 0644));

   cleanup_fd(fd);

   return 0;
}
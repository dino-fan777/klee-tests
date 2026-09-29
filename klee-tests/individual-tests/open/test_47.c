/*
 * test_47.c - Open fd table full, returns EMFILE?
 *
 * KLEE has MAX_FDS=32. Fds 0,1,2 are stdin/stdout/stderr. fd 3 is our
 * open. dup to fill remaining 28 slots. Next open should fail EMFILE.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_47.c
 * Run    : klee --posix-runtime --libc=uclibc test_47.bc --sym-files 1 10
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

   int fd = open(fname, flags);
   __gen_assert(open_succeeds(fd));

   //fill all remaining fd slots with dup
   int last_dup = -1;
   for (int i = 0; i < 28; i++) {
      last_dup = dup(fd);
      if (last_dup < 0) break;
   }

   //next open should fail EMFILE
   int fd_full = open(fname, O_RDONLY);
   __gen_assert(open_fails(fd_full));

   //cleanup all fds
   for (int i = 3; i < 32; i++) close(i);
   return 0;
}
/*
 * test_11.c - Seek on invalid fd (-1) fails (EBADF)
 *
 * Only pure lseek test - no open, no file setup.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   off_t pos = lseek(-1, 0, SEEK_SET);
   __gen_assert(lseek_fails(pos));

   return 0;
}
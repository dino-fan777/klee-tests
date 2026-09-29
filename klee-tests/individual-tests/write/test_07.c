/*
 * test_07.c - write to invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 1
 */
#include "test_helper.h"

int main(void) {
   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   ssize_t wret = write(-1, "hello", 5);
   __gen_assert(write_error(wret));

   return 0;
}
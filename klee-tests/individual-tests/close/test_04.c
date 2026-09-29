/*
 * test_04.c - close invalid fd (99) fails (EBADF)
 *
 * fd way out of range (KLEE max is 32 due to size of the fd array).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc --sym-files 1 10
 */
#include "test_helper.h"

int main(void) {
   /* Create the symbolic file through the API rather than relying on
      KLEE's --sym-files option, which would tie this suite to one tool.
      The descriptor is closed immediately so that the first open() below
      still receives fd 3. */
   cleanup_fd(__file_create("A_data"));
   int cret = close(99);
   __gen_assert(close_fails(cret));

   return 0;
}
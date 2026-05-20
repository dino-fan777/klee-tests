/*
 * test_06.c - chmod 0000 removes all permissions
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0000);
   assert_perms(0000);

   return 0;
}
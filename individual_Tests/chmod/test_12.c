/*
 * test_15.c - chmod twice, second overwrites first
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_15.c
 * Run    : klee --posix-runtime --libc=uclibc test_15.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0777);
   assert_chmod_succeeds(0444);
   assert_perms(0444);

   return 0;
}
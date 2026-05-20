/*
 * test_07.c - chmod 0755 sets rwxr-xr-x
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0755);
   assert_perms(0755);

   return 0;
}
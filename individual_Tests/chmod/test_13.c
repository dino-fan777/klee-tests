/*
 * test_16.c - chmod before open gives single completed path
 *
 * Without chmod, open() forks on symbolic st_mode. With chmod, st_mode
 * is concrete only 1 completed path, 0 partial paths.
 * The test is: Check KLEE output: "completed paths = 1"
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_16.c
 * Run    : klee --posix-runtime --libc=uclibc test_16.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assert_chmod_succeeds(0666);
   assume_flags(O_RDWR);

   int fd = open(fname, flags);
   assert_open_succeeds(fd);

   printf("[PASS] chmod before open - kinda single path execution\n");

   cleanup_fd(fd);
   return 0;
}
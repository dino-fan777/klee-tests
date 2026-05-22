/*
 * test_11.c - KLEE QUIRK: dup'd fds do NOT share offset
 *
 * On a real kernel, dup'd fds share the same file offset, writing on one
 * advances the offset for both. KLEE does *f2 = *f which COPIES the offset
 * at dup time but doesn't link them. After dup, each fd has its own offset.
 *
 * Code comment: "XXX Incorrect, really we need another data structure
 * for open files"
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
   declare_symbolic_fname();
   declare_symbolic_flags();

   assume_file_exists();
   assume_flags(O_RDWR);

   int fd1 = open(fname, flags, 0644);
   assert_open_succeeds(fd1);

   int fd2 = assert_dup_succeeds(fd1);

   //both start at offset 0
   assert_lseek(fd1, 0, SEEK_CUR, 0);
   assert_lseek(fd2, 0, SEEK_CUR, 0);

   //write 5 bytes on fd1, moves fd1 offset to 5
   assert_write_succeeds(fd1, "hello", 5);
   assert_lseek(fd1, 0, SEEK_CUR, 5);

   //KLEE QUIRK: fd2 offset is still 0 (not shared), on real kernel this would be 5 
   assert_lseek(fd2, 0, SEEK_CUR, 0);

   printf("[PASS] KLEE quirk confirmed: fd2 offset not shared with fd1\n");

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
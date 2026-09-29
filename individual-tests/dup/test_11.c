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
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDWR));

   int fd1 = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd1));

   int fd2 = dup(fd1);
   __gen_assert(dup_is_new(fd2, fd1));

   //both start at offset 0
   off_t pos1 = lseek(fd1, 0, SEEK_CUR);
   __gen_assert(lseek_is(pos1, 0));
   off_t pos2 = lseek(fd2, 0, SEEK_CUR);
   __gen_assert(lseek_is(pos2, 0));

   //write 5 bytes on fd1, moves fd1 offset to 5
   ssize_t wret = write(fd1, "hello", 5);
   __gen_assert(write_all(wret, 5));
   off_t pos3 = lseek(fd1, 0, SEEK_CUR);
   __gen_assert(lseek_is(pos3, 5));

   //KLEE QUIRK: fd2 offset is still 0 (not shared), on real kernel this would be 5
   off_t pos4 = lseek(fd2, 0, SEEK_CUR);
   __gen_assert(lseek_is(pos4, 0));

   printf("[PASS] KLEE quirk confirmed: fd2 offset not shared with fd1\n");

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
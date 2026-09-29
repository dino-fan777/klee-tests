/*
 * test_10.c - O_APPEND write goes to end of file, verify offset
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc --sym-files 1 1
 */
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;
   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_APPEND | O_WRONLY));

   int fd = open(fname, flags, 0644);
   __gen_assert(open_succeeds(fd));

   off_t original_size = lseek(fd, 0, SEEK_END);
   printf("[info] original file size = %d\n", klee_get_value_i32(original_size));

   ssize_t wret = write(fd, "abc", 3);
   __gen_assert(write_all(wret, 3));

   off_t current = lseek(fd, 0, SEEK_CUR);
   __gen_assert(lseek_is(current, original_size + 3));

   cleanup_fd(fd);
   return 0;
}
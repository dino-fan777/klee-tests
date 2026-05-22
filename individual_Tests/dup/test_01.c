/*
 * test_01.c - dup on valid fd succeeds, returns new fd
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_01.c
 * Run    : klee --posix-runtime --libc=uclibc test_01.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_exists();
    assume_flags(O_RDONLY);

    int fd = open(fname, flags, 0644);
    assert_open_succeeds(fd);

    int fd2 = assert_dup_succeeds(fd);

    cleanup_fd(fd);
    cleanup_fd(fd2);
    return 0;
}
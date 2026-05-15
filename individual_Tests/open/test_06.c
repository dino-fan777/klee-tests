/*
 * test_06.c - O_RDWR on non-existing file fails (no O_CREAT)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_not_exists();
    assume_flags(O_RDWR);

    int fd = open(fname, flags, 0644);

    assert_open_fails(fd);

    cleanup_fd(fd);
    return 0;
}
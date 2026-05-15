/*
 * test_14.c - O_NONBLOCK | O_RDONLY on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_14.c
 * Run    : klee --posix-runtime --libc=uclibc test_14.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_exists();
    assume_flags(O_NONBLOCK | O_RDONLY);

    int fd = open(fname, flags, 0644);

    assert_open_succeeds(fd);

    cleanup_fd(fd);
    return 0;
}
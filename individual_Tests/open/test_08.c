/*
 * test_08.c - O_CREAT | O_RDWR creates new file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_not_exists();
    assume_flags(O_CREAT | O_RDWR);

    int fd = open(fname, flags, 0644);

    assert_open_succeeds(fd);

    cleanup_fd(fd);
    return 0;
}
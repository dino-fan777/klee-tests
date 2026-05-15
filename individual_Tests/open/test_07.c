/*
 * test_07.c - O_CREAT | O_WRONLY creates new file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_not_exists();
    assume_flags(O_CREAT | O_WRONLY);

    int fd = open(fname, flags, 0644);

    assert_open_succeeds(fd);

    cleanup_fd(fd);
    return 0;
}
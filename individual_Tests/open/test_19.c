/*
 * test_19.c - O_RDWR on read-only permission file fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone test_19.c
 * Run    : klee --posix-runtime --libc=uclibc test_19.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_exists();
    assume_flags(O_RDWR);

    int fd = open(fname, flags, 0644);

    assert_open_fails(fd);

    cleanup_fd(fd);
    return 0;
}
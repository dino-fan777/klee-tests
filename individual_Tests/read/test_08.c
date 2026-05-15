/*
 * test_08.c - read more bytes than file has (partial read / EOF / sym files has 10 we read 20)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc --sym-files 1 10
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_exists();
    assume_flags(O_RDONLY);

    int fd = open(fname, flags, 0644);
    assert_open_succeeds(fd);

    char buf[20] = {0};
    assert_read_at_most(fd, buf, 20);

    cleanup_fd(fd);
    return 0;
}
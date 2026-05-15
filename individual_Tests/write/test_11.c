/*
 * test_11.c - two sequential writes, verify total bytes via lseek
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I.. test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

int main(void) {
    declare_symbolic_fname();
    declare_symbolic_flags();

    assume_file_exists();
    assume_flags(O_RDWR | O_TRUNC);

    int fd = open(fname, flags, 0644);
    assert_open_succeeds(fd);

    assert_write_succeeds(fd, "abc", 3);
    assert_write_succeeds(fd, "def", 3);

    off_t size = lseek(fd, 0, SEEK_CUR);
    int c_size = klee_get_value_i32(size);

    if (c_size != 6) {
        printf("  [debug] expected offset=6 after two writes, got=%d\n", c_size);
        klee_report_error(__FILE__, __LINE__,
            "two 3-byte writes should produce offset 6", "test_fail");
    }
    printf("[PASS] two sequential writes — total offset = %d\n", c_size);

    cleanup_fd(fd);
    return 0;
}
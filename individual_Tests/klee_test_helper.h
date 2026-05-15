#ifndef KLEE_TEST_HELPER_H
#define KLEE_TEST_HELPER_H

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <klee/klee.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_FDS       29
#define MAX_SYM_FILES 26

/*
KLEE has a max limit of 29 fd's: 32-3 = 29
How files are created: https://github.com/klee/klee/blob/master/runtime/POSIX/fd_init.c
Sym-files named 'A'+k: https://github.com/klee/klee/blob/master/tools/klee-replay/file-creator.c

stat() IS intercepted by KLEE's POSIX runtime (__fd_stat in fd.c).
BUT stat() returns 0 for ANY existing path, not just sym-files.
For example '/' (root dir, ASCII 47) always exists.
So we constrain fname[0] to uppercase letters BEFORE calling stat(),
ensuring we only match actual sym-file names (A-Z).

fname and flags are file-scope globals so every function can access them without arguments.

Proves O_CREAT won't work since a file always exists from the console:
https://github.com/klee/klee/blob/master/runtime/POSIX/klee_init_env.c
https://github.com/klee/klee/blob/master/runtime/POSIX/fd_init.c

file limit is until \x7f in ASCII then KLEE crashes since there is no upper bounds.

partial writes/reads are not really supported in KLEE, whatever you request to write that is the number that will be returned. i still debug it regardless

on write tests we cannot use --sym-files 1 1, since we write actual data we do --sym-files 1 10

not possible to guarantee only 1 path sucess and 0 partially completed paths in either open/write/read since I have no control over the symbolic file st_mode during runtime UNLESS we use the chmod syscall

write does not check the O_ACCMODE of a fd therefore regardless if we open a file with READ ONLY it will write as long as the symbolic file has the permissions to write.
same happens in read.

if we use O_TRUNC the symbolic values should be wiped? then if we write on the file it the data should be concrete

O_TRUNC does not update the dfile->size when write happens. With --sym-files 1 10, the sym-file is created with dfile->size = 10. Then:
1. open(O_TRUNC) - should set dfile->size = 0
2. write("test", 4) - should update dfile->size = 4
3. lseek(0, SEEK_END) - computes dfile->size + 0    write() implementation likely only advances f->off (the offset) without updating dfile->size. So the chain becomes:                                                               dfile->size = 10          ← created by --sym-files 1 10
open(O_TRUNC)             ← may or may not set dfile->size = 0
write("test", 4)          ← advances f->off to 4, does NOT update dfile->size
lseek(0, SEEK_END)        ← returns dfile->size + 0 = 10 (not 4)            


O_TRUNC mentions?
klee@9ee06183dfb6:~/individual_Tests$ grep -rn "O_TRUNC\|TRUNC" /tmp/klee_src/runtime/POSIX/
/tmp/klee_src/runtime/POSIX/fd.c:156:    if ((flags & O_TRUNC) && (flags & O_RDONLY)) {
/tmp/klee_src/runtime/POSIX/fd.c:157:      /* The result of using O_TRUNC with O_RDONLY is undefined, so we
/tmp/klee_src/runtime/POSIX/fd.c:159:      klee_warning("Undefined call to open(): O_TRUNC | O_RDONLY\n");


Write how it works to in terms of capacity:

THE IF:
    Does the entire write fit in the file?
    actual_count = count;              // YES — write everything

THE ELSE BRANCH:
    File buffer:  [. . . . . . . . . .]   size = 10
    Offset:                   ^             f->off = 7
    Write request:            [x x x x x]  count = 5 (needs 7+5=12, but only 10)

    f->off + count <= dfile->size?    7 + 5 <= 10?   NO — doesn't fit

    f->off < dfile->size?             7 < 10?         YES — we're inside the file
    actual_count = 10 - 7 = 3                         write 3 bytes (what fits)

    Result:       [. . . . . . . x x x]   3 bytes written, 2 dropped
PAST OFFSET:
    File buffer:  [. . . . . . . . . .]   size = 10
    Offset:                               ^  f->off = 10 (or beyond)

    f->off < dfile->size?             10 < 10?   NO — we're at/past the end
    actual_count stays 0                          nothing written


    */

static char fname[2];
static int  flags;

/* ══════════════════════════════════════════════════════════════════════
 **** SYMBOLIC VARIABLE SETUP
 * ══════════════════════════════════════════════════════════════════════ */

static void declare_symbolic_fname(void) {
    fname[0] = 0;
    fname[1] = 0;
    klee_make_symbolic(fname, sizeof(fname), "fname");
    fname[1] = '\0';
    klee_assume(fname[0] != '\0');
}

static void declare_symbolic_flags(void) {
    klee_make_symbolic(&flags, sizeof(flags), "flags");
}

/* ══════════════════════════════════════════════════════════════════════
 **** FILE EXISTENCE
 *
 * assume_file_exists:     constrains fname to an EXISTING sym-file.
 *                         First limits fname[0] to 'A'-'Z' (only valid
 *                         sym-file names), THEN uses stat() to confirm
 *                         the file exists. This prevents false matches
 *                         on paths like '/' or '.' which also pass stat().
 *
 * (NOT POSSIBLE) assume_file_not_exists: constrains fname so it does NOT match any
 *                         existing file. Uses stat() to confirm.
 *
 * ══════════════════════════════════════════════════════════════════════ */

static void assume_file_exists(void) {

    klee_assume(fname[0] >= 'A' && fname[0] <= 'Z');

    struct stat sb;
    int r = stat(fname, &sb);
    klee_assume(r == 0);

    printf("[assume_file_exists] fname='%c' (0x%02x) - file confirmed via stat()\n",
           klee_get_value_i32(fname[0]), klee_get_value_i32(fname[0]));
}

//Not possible to do but here is my attempts:
static void assume_file_not_exists(void) {
    //struct stat sb;
    //int r = stat(fname, &sb);
    //klee_assume(r < 0);

    klee_assume(fname[0] >= 'a' && fname[0] <= 'z');
 
    printf("[assume_file_not_exists] fname='%c' (0x%02x) - not a sym-file\n",
           klee_get_value_i32(fname[0]), klee_get_value_i32(fname[0]));


    /*printf("[assume_file_not_exists] fname='%c' (0x%02x) — no such file\n",
           klee_get_value_i32(fname[0]), klee_get_value_i32(fname[0])); */
}

/* ══════════════════════════════════════════════════════════════════════
 **** FLAG CONSTRAINTS & PERMISSIONS
 * ══════════════════════════════════════════════════════════════════════ */

static void assume_flags(int val) {
    klee_assume(flags == val);
}

//if we want 1 completed path only and 0 no complete we use this to change the file st_mode 
//chmod(fname, 0666);  https://github.com/klee/klee/blob/master/runtime/POSIX/fd.c:619
static void set_file_permissions(mode_t mode) {
    chmod(fname, mode);
}

/* ══════════════════════════════════════════════════════════════════════
 **** DEBUG OUTPUT
 *
 * print_open_debug: prints concrete values for fd, flags, fname, errno
 *                   using klee_get_value_i32 to concretize everything.
 *
 * ══════════════════════════════════════════════════════════════════════ */

static void print_open_debug(int fd) {
    int c_fd    = klee_get_value_i32(fd);
    int c_flags = klee_get_value_i32(flags);
    int c_fname = klee_get_value_i32(fname[0]);
    int c_errno = klee_get_value_i32(errno);

    printf("  [debug] fd=%d | flags=0x%x | fname='%c' (0x%02x) | errno=%d\n",
           c_fd, c_flags, c_fname, c_fname, c_errno);

    //klee_print_range("fd range", fd);
    //klee_print_range("errno range", errno);
}

/* ══════════════════════════════════════════════════════════════════════
 **** ASSERTIONS
 *
 * assert_open_succeeds: expects fd >= 0
 * assert_open_fails:    expects fd < 0
 *
 * ══════════════════════════════════════════════════════════════════════ */

static void assert_open_succeeds(int fd) {
    if (fd < 0) {
        klee_warning("UNEXPECTED: open() failed");
        print_open_debug(fd);
        klee_report_error(__FILE__, __LINE__,
            "expected open() to SUCCEED but it FAILED",
            "test_fail");
    } else {
        printf("[PASS] open() succeeded as expected\n");
        print_open_debug(fd);
    }
}

static void assert_open_fails(int fd) {
    if (fd >= 0) {
        klee_warning("UNEXPECTED: open() succeeded");
        print_open_debug(fd);
        klee_report_error(__FILE__, __LINE__,
            "expected open() to FAIL but it SUCCEEDED",
            "test_fail");
    } else {
        printf("[PASS] open() failed as expected\n");
        print_open_debug(fd);
    }
}

static void assert_write_succeeds(int fd, const char *buf, size_t len) {
    ssize_t ret = write(fd, buf, len);
    int c_ret   = klee_get_value_i32(ret);
    int c_errno = klee_get_value_i32(errno);
 
    if (ret < 0) {
        printf("  [debug] write ret=%d | errno=%d\n", c_ret, c_errno);
        klee_print_range("errno range", errno);
        klee_report_error(__FILE__, __LINE__,
            "expected write() to SUCCEED but it FAILED",
            "test_fail");
    } else if ((size_t)ret != len) {
        printf("  [debug] partial write: expected=%zu got=%d\n", len, c_ret);
        klee_report_error(__FILE__, __LINE__,
            "partial write — not all bytes written",
            "test_fail");
    } else {
        printf("[PASS] write() — %d/%zu bytes written\n", c_ret, len);
    }
}
 
static void assert_write_fails(int fd, const char *buf, size_t len) {
    ssize_t ret = write(fd, buf, len);
    int c_ret   = klee_get_value_i32(ret);
    int c_errno = klee_get_value_i32(errno);
 
    if (ret >= 0) {
        printf("  [debug] write ret=%d | errno=%d\n", c_ret, c_errno);
        klee_report_error(__FILE__, __LINE__,
            "expected write() to FAIL but it SUCCEEDED",
            "test_fail");
    } else {
        printf("[PASS] write() failed as expected - errno=%d\n", c_errno);
    }
}

static void assert_read_succeeds(int fd, char *buf, size_t len) {
    ssize_t ret = read(fd, buf, len);
    int c_ret   = klee_get_value_i32(ret);
    int c_errno = klee_get_value_i32(errno);
 
    if (ret < 0) {
        printf("  [debug] read ret=%d | errno=%d\n", c_ret, c_errno);
        klee_print_range("errno range", errno);
        klee_report_error(__FILE__, __LINE__,
            "expected read() to SUCCEED but it FAILED",
            "test_fail");
    } else if ((size_t)ret != len) {
        printf("  [debug] partial read: expected=%zu got=%d\n", len, c_ret);
        klee_report_error(__FILE__, __LINE__,
            "partial read — not all bytes read",
            "test_fail");
    } else {
        printf("[PASS] read() — %d/%zu bytes read\n", c_ret, len);
    }
}
 
static void assert_read_fails(int fd, char *buf, size_t len) {
    ssize_t ret = read(fd, buf, len);
    int c_ret   = klee_get_value_i32(ret);
    int c_errno = klee_get_value_i32(errno);
 
    if (ret >= 0) {
        printf("  [debug] read ret=%d | errno=%d\n", c_ret, c_errno);
        klee_report_error(__FILE__, __LINE__,
            "expected read() to FAIL but it SUCCEEDED",
            "test_fail");
    } else {
        printf("[PASS] read() failed as expected - errno=%d\n", c_errno);
    }
}

static void assert_buffers_match(const char *expected, const char *actual, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (expected[i] != actual[i]) {
            printf("  [debug] mismatch at byte %zu: expected=0x%02x got=0x%02x\n",
                   i, (unsigned char)expected[i], (unsigned char)actual[i]);
            klee_report_error(__FILE__, __LINE__,
                "buffer mismatch - written data != read data",
                "test_fail");
        }
    }
    printf("[PASS] buffers match — %zu bytes verified\n", len);
}

//assert_read_at_most only checks ret >= 0 any non-negative return is fine unlike assert_read_succeeds
static void assert_read_at_most(int fd, char *buf, size_t len) {
    ssize_t ret = read(fd, buf, len);
    int c_ret   = klee_get_value_i32(ret);
    int c_errno = klee_get_value_i32(errno);

    if (ret < 0) {
        printf("  [debug] read ret=%d | errno=%d\n", c_ret, c_errno);
        klee_report_error(__FILE__, __LINE__,
            "expected read() to succeed but it failed",
            "test_fail");
    } else {
        printf("[PASS] read() - %d/%zu bytes read (at most)\n", c_ret, len);
    }
}

/* ══════════════════════════════════════════════════════════════════════
 **** LSEEK ASSERTIONS
 *
 * assert_lseek:           calls lseek, checks ret == expected offset
 * assert_lseek_fails:     calls lseek, checks ret == -1
 * assert_seek_and_read:   seeks to offset, reads 1 byte, verifies match
 *
 * ══════════════════════════════════════════════════════════════════════ */

static const char* whence_to_str(int whence) {
    switch (whence) {
        case SEEK_SET: return "SEEK_SET";
        case SEEK_CUR: return "SEEK_CUR";
        case SEEK_END: return "SEEK_END";
        default:       return "???";
    }
}

static void assert_lseek(int fd, off_t offset, int whence, off_t expected) {
    off_t pos  = lseek(fd, offset, whence);
    int c_pos  = klee_get_value_i32(pos);
    int c_exp  = klee_get_value_i32(expected);

    if (c_pos != c_exp) {
        printf("  [debug] lseek(%d, %s) returned %d, expected %d\n",
               klee_get_value_i32(offset), whence_to_str(whence), c_pos, c_exp);
        klee_report_error(__FILE__, __LINE__,
            "lseek returned unexpected offset", "test_fail");
    }
    printf("[PASS] lseek(%d, %s) returned %d\n",
           klee_get_value_i32(offset), whence_to_str(whence), c_pos);
}

//TODO: also check the expected?
static void assert_lseek_fails(int fd, off_t offset, int whence) {
    off_t pos   = lseek(fd, offset, whence);
    int c_pos   = klee_get_value_i32(pos);
    int c_errno = klee_get_value_i32(errno);

    if (c_pos != -1) {
        printf("  [debug] lseek(%d, %s) returned %d, expected -1\n",
               klee_get_value_i32(offset), whence_to_str(whence), c_pos);
        klee_report_error(__FILE__, __LINE__,
            "lseek should have failed but succeeded", "test_fail");
    }
    printf("[PASS] lseek(%d, %s) failed as expected - errno=%d\n",
           klee_get_value_i32(offset), whence_to_str(whence), c_errno);
}

static void assert_seek_and_read(int fd, off_t offset, char expected_byte) {
    lseek(fd, offset, SEEK_SET);

    char buf[1] = {0};
    assert_read_succeeds(fd, buf, 1);

    if (buf[0] != expected_byte) {
        printf("  [debug] at offset %d: expected='%c' got='%c'\n",
               klee_get_value_i32(offset), expected_byte, buf[0]);
        klee_report_error(__FILE__, __LINE__,
            "byte at seeked position does not match expected", "test_fail");
    }
    printf("[PASS] byte at offset %d = '%c'\n",
           klee_get_value_i32(offset), expected_byte);
}


/* ══════════════════════════════════════════════════════════════════════
 **** CLEANUP
 * ══════════════════════════════════════════════════════════════════════ */

static void cleanup_fd(int fd) {
    if (fd >= 0) close(fd);
}

#endif
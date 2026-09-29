#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "klee/file_api.h" 

/* ══════════════════════════════════════════════════════════════════════
 **** SYMBOLIC VARIABLE SETUP
 * ══════════════════════════════════════════════════════════════════════ */

static void declare_symbolic_file_name(char *fname) {
    fname[0] = (char) __sym_var_named("fname", 8);
    fname[1] = '\0';
    __assume(_NEQ_(fname[0], '\0'));                 
}

static int declare_symbolic_flags(void) {
    return (int) __sym_var_named("flags", 32);
}

/* ══════════════════════════════════════════════════════════════════════
 **** OPEN
 * ══════════════════════════════════════════════════════════════════════ */

static cnstr_t open_succeeds(int fd) {
    return _GE_(fd, 0);
}

static cnstr_t open_fails(int fd) {
    return _LT_(fd, 0);
}

static cnstr_t access_mode_is(int fd, int mode) {
    return _EQ_(__file_flags(fd) & O_ACCMODE, mode & O_ACCMODE);
}

static cnstr_t offset_is(int fd, off_t off) {
    return _EQ_(__file_offset(fd), off);
}

static cnstr_t fd_is(int fd, int expected) {
    return _EQ_(fd, expected);
}

static cnstr_t flags_equal(int flags, int value){
    return _EQ_(flags, value);
}

/* ══════════════════════════════════════════════════════════════════════
 **** WRITE
 * ══════════════════════════════════════════════════════════════════════ */

static cnstr_t write_all(ssize_t ret, size_t len) { 
    return _EQ_(ret, (ssize_t)len); 
}

static cnstr_t write_partial(ssize_t ret, size_t len) { 
    return _AND_(_GE_(ret, 0), _LT_(ret, (ssize_t)len)); 
}

static cnstr_t write_error(ssize_t ret) { 
    return _LT_(ret, 0); 
}

/* ══════════════════════════════════════════════════════════════════════
 **** READ
 * ══════════════════════════════════════════════════════════════════════ */

static cnstr_t read_all(ssize_t ret, size_t len) { 
    return _EQ_(ret, (ssize_t)len); 
}

static cnstr_t read_partial(ssize_t ret, size_t len) { 
    return _AND_(_GE_(ret, 0), _LT_(ret, (ssize_t)len)); 
}

static cnstr_t read_error(ssize_t ret) { 
    return _LT_(ret, 0); 
}

static cnstr_t read_at_most(ssize_t ret) { 
    return _GE_(ret, 0); 
}
 
// buffers match byte-by-byte, as a single AND-ed constraint
//a[0]==b[0]  AND  a[1]==b[1]  AND  a[2]==b[2]  AND  a[3]==b[3]
static cnstr_t buffers_match(const char *a, const char *b, size_t len) {
    cnstr_t acc = _EQ_(0, 0);
    size_t i;
    for (i = 0; i < len; i++)
        acc = _AND_(acc, _EQ_(a[i], b[i]));
    return acc;
}

/* ══════════════════════════════════════════════════════════════════════
 **** LSEEK
 * ══════════════════════════════════════════════════════════════════════ */

static cnstr_t lseek_is(off_t pos, off_t expected) { 
    return _EQ_(pos, expected); 
}
static cnstr_t lseek_fails(off_t pos) { 
    return _EQ_(pos, -1); 
}

/* ══════════════════════════════════════════════════════════════════════
 **** CLOSE
 * ══════════════════════════════════════════════════════════════════════ */

static cnstr_t close_succeeds(int ret) { 
    return _EQ_(ret, 0); 
}

static cnstr_t close_fails(int ret) { 
    return _EQ_(ret, -1); 
}

/* ══════════════════════════════════════════════════════════════════════
 **** DUP / DUP2
 * ══════════════════════════════════════════════════════════════════════ */
 
static cnstr_t dup_succeeds(int ret) { 
    return _GE_(ret, 0); 
}

static cnstr_t dup_is_new(int ret, int oldfd) { 
    return _AND_(_GE_(ret, 0), _NEQ_(ret, oldfd)); 
}

static cnstr_t dup_fails(int ret) { 
    return _EQ_(ret, -1); 
}

static cnstr_t dup2_returns(int ret, int newfd) { 
    return _EQ_(ret, newfd); 
}

static cnstr_t dup2_fails(int ret) { 
    return _EQ_(ret, -1); 
}

/* ══════════════════════════════════════════════════════════════════════
 **** ERRNO PREDICATE   (UNVERIFIED: isolation-test __get_errno first)
 * ══════════════════════════════════════════════════════════════════════ */
 
static cnstr_t errno_is(int expected) { return _EQ_(__get_errno(), expected); }

/* ══════════════════════════════════════════════════════════════════════
 **** CHMOD
 * ══════════════════════════════════════════════════════════════════════ */
 
static cnstr_t chmod_succeeds(int ret) { 
    return _EQ_(ret, 0); 
}

static cnstr_t chmod_fails(int ret) { 
    return _EQ_(ret, -1); 
}

static cnstr_t perms_are(int fd, mode_t expected) { 
    return _EQ_(__file_mode(fd) & 0777, expected & 0777); 
}

/* ══════════════════════════════════════════════════════════════════════
 **** DEBUG  (opt-in into the tests)
 * Example usage:
 * 
 * int fd = open(fname, flags);
 * debug_fd(fd); <--- add this to the test
 * 
 * ssize_t ret = write(fd, buf, 5);
 * debug_ret("write", ret); <--- add this to the test
 * ══════════════════════════════════════════════════════════════════════ */
 
static void debug_fd(int fd) {
    printf("  [debug] fd=%ld | flags=0x%lx | offset=%ld | errno=%ld\n",
           __concretize(fd),
           __concretize(__file_flags(fd)),
           __concretize(__file_offset(fd)),
           __concretize(__get_errno()));
}
 
static void debug_ret(const char *what, ssize_t ret) {
    printf("  [debug] %s ret=%ld | errno=%ld\n",
           what, __concretize(ret), __concretize(__get_errno()));
}
 
static void debug_name(const char *fname) {
    printf("  [debug] fname='%c' (0x%02lx)\n",
           (char) __concretize(fname[0]), __concretize(fname[0]));
}

/* ══════════════════════════════════════════════════════════════════════
 **** CLEANUP
 * ══════════════════════════════════════════════════════════════════════ */

static void cleanup_fd(int fd) {
    if (fd >= 0)
        close(fd);
}

#endif
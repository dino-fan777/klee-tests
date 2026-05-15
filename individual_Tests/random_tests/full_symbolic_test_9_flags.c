#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <klee/klee.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
    char fname[2] = {0};
    klee_make_symbolic(fname, sizeof(fname), "fname");

    int flags;
    klee_make_symbolic(&flags, sizeof(flags), "flags");

    //3 access modes + 6 modifier flags
    int valid_bits = O_RDONLY | O_WRONLY | O_RDWR     // access modes
                   | O_CREAT | O_EXCL | O_TRUNC       // creation-time flags
                   | O_APPEND | O_NONBLOCK | O_CLOEXEC; // status flags

    klee_assume((flags & ~valid_bits) == 0);

    //klee_assume((flags & O_ACCMODE) != 0x3);

    //O_EXCL requires O_CREAT to be meaningful
    klee_assume(!(flags & O_EXCL) || (flags & O_CREAT));

    //O_TRUNC doesn't make sense with O_RDONLY
    klee_assume(!(flags & O_TRUNC) || (flags & O_ACCMODE) != O_RDONLY);

    fname[sizeof(fname) - 1] = '\0';
    klee_assume(fname[0] != '\0');

    int fd = open(fname, flags, 0644);
    if (fd < 0) {
        klee_warning("open() failed on this path");
        klee_print_expr("fd", fd);
        klee_print_expr("errno", errno);
        klee_print_range("errno range", errno);
        printf("open failed: flags=0x%x fname='%c' errno=%d\n",
               klee_get_value_i32(flags),
               klee_get_value_i32(fname[0]),
               klee_get_value_i32(errno));
        return 1;
    }

    printf("open succeeded: fd=%d flags=0x%x\n",
           fd, klee_get_value_i32(flags));
    close(fd);
    return 0;
}

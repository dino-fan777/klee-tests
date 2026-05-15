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
   int valid_bits = O_RDONLY | O_WRONLY | O_RDWR;
   
   klee_assume((flags & ~valid_bits) == 0); //all other bits are 0, we lose several flags (also for example O_EXECL only meaningful with O_CREAT, etc...)

   fname[sizeof(fname) - 1] = '\0'; //force last character of the filename to be null
   
   klee_assume(fname[0] != '\0');   //doesn't start with a null byte
   //klee_assume(fname[0] >= 0x20 && fname[0] <= 0x7E);  //printable ASCI

   int fd = open(fname, flags, 0644);

    if (fd < 0){

      klee_warning("open() failed on this path");
      klee_print_expr("fd", fd);
      klee_print_expr("errno", errno);             //errno: 13 - EACCES, permission denied
      klee_print_range("errno range", errno);

      printf("open failed: flags=0x%x fname='%c' errno=%d\n", klee_get_value_i32(flags), klee_get_value_i32(fname[0]), klee_get_value_i32(errno));
      return 1;
   }

   //assert(fd < 32);

   printf("open succeeded: fd=%d flags=0x%x\n", fd, klee_get_value_i32(flags));
   close(fd);

   return 0;   
}

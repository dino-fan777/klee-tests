#include "klee_test_helper.h"

int main(void) {
   int fd1 = open("A", O_RDONLY, 0644);
   printf("fd1=%d\n", fd1);

   close(fd1);

   int fd2 = open("A", O_RDONLY, 0644);
   printf("fd2=%d errno=%d\n", fd2, errno);

   return 0;
}
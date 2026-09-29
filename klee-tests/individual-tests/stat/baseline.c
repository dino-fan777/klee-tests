/*
 * baseline.c - how does the stat summary populate a concrete buffer with a concrete file?
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone baseline.c
 * Run    : klee --posix-runtime --libc=uclibc baseline.bc --sym-files 1 1
 */
#include "klee_test_helper.h"

/*
OUTPUT with syscall concretized:

File info:
Device ID: 138
Inode number: 2627922
Mode: 100777
Hard links: 1
Owner UID: 1000
Group GID: 1000
Special device ID: 0
Total size: 44 bytes
Block size: 4096
Blocks allocated: 8
Last access: 1779125838
Last modification: 1779125838
Last status change: 1779127211
File type:
regular file

Output with symbolic syscall:

Device ID        : 0
Inode number     : -4294967295
Mode             : 0
Hard links       : 0
Owner UID        : 0
Group GID        : 0
Special device   : 0
Size             : 1 bytes
Block size       : 0 bytes
Blocks allocated : 8
Access time      : 0
Modify time      : 0
Change time      : 0

klee_make_symbolic(s, sizeof(*s), sname);
klee_assume((s->st_ino & 0x7FFFFFFF) != 0);
klee_assume((s->st_blksize & ~0xFFFF) == 0);
klee_prefer_cex(s, !(s->st_mode & ~(S_IFMT | 0777)));
klee_prefer_cex(s, s->st_dev == defaults->st_dev);
klee_prefer_cex(s, s->st_rdev == defaults->st_rdev);
klee_prefer_cex(s, (s->st_mode&0700) == 0600);
klee_prefer_cex(s, (s->st_mode&0070) == 0040);
klee_prefer_cex(s, (s->st_mode&0007) == 0004);
klee_prefer_cex(s, (s->st_mode&S_IFMT) == S_IFREG);
klee_prefer_cex(s, s->st_nlink == 1);
klee_prefer_cex(s, s->st_uid == defaults->st_uid);
klee_prefer_cex(s, s->st_gid == defaults->st_gid);
klee_prefer_cex(s, s->st_blksize == 4096);
klee_prefer_cex(s, s->st_atime == defaults->st_atime);
klee_prefer_cex(s, s->st_mtime == defaults->st_mtime);
klee_prefer_cex(s, s->st_ctime == defaults->st_ctime);
s->st_size = dfile->size;
s->st_blocks = 8;
dfile->stat = s;

The defaults are:

struct stat64 s;
for (k=0; k < n_files; k++) {
    name[0] = 'A' + k;
    __create_new_dfile(&__exe_fs.sym_files[k], file_length, name, &s);
  }

All 0's.

Mode is still 0 until it constrains?
*/

static void dump_stat(struct stat *sb) {

   printf("==== FINAL STAT DUMP (KLEE CONCRETE VIEW) ====\n");

   long dev   = klee_get_value_i64((long)sb->st_dev);
   long ino   = klee_get_value_i64((unsigned long)sb->st_ino);
   int mode   = klee_get_value_i32(sb->st_mode);
   int links  = klee_get_value_i32(sb->st_nlink);
   int uid    = klee_get_value_i32(sb->st_uid);
   int gid    = klee_get_value_i32(sb->st_gid);
   long rdev  = klee_get_value_i64((long)sb->st_rdev);
   long size  = klee_get_value_i64((long)sb->st_size);
   long blksz = klee_get_value_i64((long)sb->st_blksize);
   long blocks= klee_get_value_i64((long)sb->st_blocks);

   long atime = klee_get_value_i64((long)sb->st_atime);
   long mtime = klee_get_value_i64((long)sb->st_mtime);
   long ctime = klee_get_value_i64((long)sb->st_ctime);

   printf("Device ID        : %ld\n", dev);
   printf("Inode number     : 0x%lx\n", ino);
   printf("Mode             : %o\n", mode);
   printf("Hard links       : %d\n", links);
   printf("Owner UID        : %d\n", uid);
   printf("Group GID        : %d\n", gid);
   printf("Special device   : %ld\n", rdev);
   printf("Size             : %ld bytes\n", size);
   printf("Block size       : %ld bytes\n", blksz);
   printf("Blocks allocated : %ld\n", blocks);

   printf("Access time      : %ld\n", atime);
   printf("Modify time      : %ld\n", mtime);
   printf("Change time      : %ld\n", ctime);

   printf("==============================================\n");
}

int main(void){

   //32 or 64?
   struct stat sb;

   //if we "A" we have to use klee --posix-runtime --libc=uclibc baseline.bc --sym-files 1 1
   //if we use the "test.txt" we can use the above or without symfiles, the syscall will be concretized regardless
   if(stat("A", &sb) == -1){
      klee_report_error(__FILE__, __LINE__,
                "couldn't stat the file",
                "test_fail");
   }

   dump_stat(&sb);

   return 0;
}
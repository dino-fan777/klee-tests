# Open()

## full_symbolic_test_3_flags.c

Compile:

	clang -emit-llvm -c -g full_symbolic_test_3_flags.c -o full_symbolic_test_3_flags.bc

Run:

	klee --posix-runtime --libc=uclibc full_symbolic_test_3_flags.bc --sym-files 1 10

Flags used for this test: `O_RDONLY | O_WRONLY | O_RDWR`, with permissions `0644`.

- **O_RDONLY**: 0x0
- **O_WRONLY**: 0x1
- **O_RDWR**: 0x2

KLEE output:

```
KLEE: WARNING ONCE: calling external: syscall(16, 0, 21505, 19851678191616) at klee_src/runtime/POSIX/fd.c:997 10
KLEE: WARNING ONCE: Alignment of memory from call "malloc" is not modelled. Using alignment of 8.
KLEE: WARNING ONCE: calling __klee_posix_wrapped_main with extra arguments.
KLEE: ERROR: full_symbolic_test.c:22: invalid klee_assume call (provably false)
KLEE: NOTE: now ignoring this error at this location
KLEE: ERROR: klee_src/runtime/POSIX/fd.c:180: external call with symbolic argument: syscall
KLEE: NOTE: now ignoring this error at this location
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(21049604968448, 2, 65, 13) at full_symbolic_test.c:34 7
open failed: flags=0x2 fname='A' errno=13
open succeeded: fd=3 flags=0x1
open succeeded: fd=3 flags=0x0
open succeeded: fd=3 flags=0x2

KLEE: done: total instructions = 30861
KLEE: done: completed paths = 4
KLEE: done: partially completed paths = 3
KLEE: done: generated tests = 6
```

In the output above we can see that we have generated 6 cases. 

### Ktest1


```
ktest file : 'klee-last/test000001.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'\x01\xff'

object 3: name: 'flags'
object 3: data: b'\x00\x00\x00\x00'
```

Replay of test 1:

	 klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000001.ktest full_symbolic_test_3_flags.bc --sym-files 1 10


Failed because of filename.

### Ktest2

```
ktest file : 'klee-out-1/test000002.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b' \xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x00\x00\x00\x00'
```

Failed because of filename, my theory is that the space made it so it tried to open(" "). 


Replay of test 2:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000002.ktest full_symbolic_test_3_flags.bc --sym-files 1 10

```
fd:4294967295
errno:2
errno range:2
KLEE: WARNING ONCE: calling external: printf(22411521556480, 0, 32, 2) at full_symbolic_test.c:34 7
open failed: flags=0x0 fname=' ' errno=2
```

### Ktest3

```
ktest file : 'klee-out-1/test000003.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: r--r--r-- //DIFF FROM ABOVE 

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x02\x00\x00\x00' //DIFF FROM ABOVE 
```

Failed because file has only read permissions and we try to open the file with read/write flags.

Replay of test 3:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000003.ktest full_symbolic_test_3_flags.bc --sym-files 1 10

```
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(11390790778880, 2, 65, 13) at full_symbolic_test.c:34 7
open failed: flags=0x2 fname='A' errno=13
```

### Ktest4

```
ktest file : 'klee-out-1/test000004.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -rw-r--r-- 

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x00\x00\x00\x00'
```

First sucess open without any flags.

Replay of test 4:


	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000004.ktest full_symbolic_test_3_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x0
```

### Ktest5

```
ktest file : 'klee-out-1/test000005.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: b'\x8a\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xff\xff\xff\xff\x01\x00\x00\x00\x00\x00\x00\x00\xa4\x81\x00\x00\xe8\x03\x00\x00\xe8\x03\x00\x00\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x10\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x7f\xea\xe0i\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x81\xea\xe0i\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x81\xea\xe0i\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff'

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x02\x00\x00\x00'
```

Another sucess open with flags.

Replay of test 5:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000005.ktest full_symbolic_test_3_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x2
```

### Ktest6

```
ktest file : 'klee-out-1/test000006.ktest'
args       : ['full_symbolic_test_3_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -rw-r--r-- 

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x01\x00\x00\x00'
```

Last test and a success with WRITE_ONLY flag.

Replay of test 6:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-1/test000006.ktest full_symbolic_test_3_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x1
``` 

## full_symbolic_test_9_flags.c

Compile:

	clang -emit-llvm -c -g full_symbolic_test_9_flags.c -o full_symbolic_test_9_flags.bc

Run:

	klee --posix-runtime --libc=uclibc full_symbolic_test_9_flags.bc --sym-files 1 10

Flags used for this test:

- **O_RDONLY**: 0x0
- **O_WRONLY**: 0x1
- **O_RDWR**: 0x2
- **O_CREAT**: 0x40
- **O_EXCL**: 0x80
- **O_TRUNC**: 0x200
- **O_APPEND**: 0x400
- **O_NONBLOCK**: 0x800
- **O_CLOEXEC**: 0x80000

Semantic constraints added:

- Access mode must be O_RDONLY, O_WRONLY, or O_RDWR (not 0x3)
- O_EXCL only allowed when O_CREAT is set
- O_TRUNC only allowed with a writable access mode

KLEE output:

```
KLEE: WARNING ONCE: calling external: syscall(16, 0, 21505, 13449282056192) at klee_src/runtime/POSIX/fd.c:997 10
KLEE: WARNING ONCE: Alignment of memory from call "malloc" is not modelled. Using alignment of 8.
KLEE: WARNING ONCE: calling __klee_posix_wrapped_main with extra arguments.
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
fd:4294967295
errno:17
errno range:17
KLEE: WARNING ONCE: calling external: printf(14647208833024, 705, 65, 17) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0x2c1 fname='A' errno=17
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
fd:4294967295
errno:17
errno range:17
open failed: flags=0xc0 fname='A' errno=17
KLEE: ERROR: klee_src/runtime/POSIX/fd.c:180: external call with symbolic argument: syscall
KLEE: NOTE: now ignoring this error at this location
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
fd:4294967295
errno:13
errno range:13
open failed: flags=0x1 fname='A' errno=13
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
fd:4294967295
fd:4294967295
errno:13
KLEE: WARNING: __klee_posix_wrapped_main: open() failed on this path
errno:13
fd:4294967295
errno range:13
errno:13
errno range:13
errno range:13
open succeeded: fd=3 flags=0x0
open failed: flags=0x241 fname='A' errno=13
open failed: flags=0x41 fname='A' errno=13
open failed: flags=0x201 fname='A' errno=13
open succeeded: fd=3 flags=0x2
open succeeded: fd=3 flags=0x1
open succeeded: fd=3 flags=0x42
open succeeded: fd=3 flags=0x202
open succeeded: fd=3 flags=0x41
open succeeded: fd=3 flags=0x241
open succeeded: fd=3 flags=0x40
open succeeded: fd=3 flags=0x201
open succeeded: fd=3 flags=0x242
```

In the output above we can see that we have generated 17 cases.

### Ktest 1 

**Same as Ktest 1 from the 3 flags file.**

### Ktest 2

**Same as Ktest 2 from the 3 flags file.**

### Ktest 3

```
ktest file : 'klee-out-11/test000003.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\xc0\x00\x00\x00'
```

Failed because of conflicting flags. Flags are set to 0xc0 which is 0x80 | 0x40. So basically **open("A", O_CREAT | O_EXCL)**. O_CREAT creates a file if it doesn't exist and O_EXCL will error if O_CREAT failed. So since 'A' exists and was created by KLEE O_CREAT will fail and O_EXCL will trigger and we get EEXIST with error code 17.

Replay of test 3:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000003.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
fd:4294967295
errno:17
errno range:17
KLEE: WARNING ONCE: calling external: printf(16305308532736, 192, 65, 17) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0xc0 fname='A' errno=17
```

### Ktest 4

```
ktest file : 'klee-out-11/test000004.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -r--r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x01\x00\x00\x00'
```

Failed. Tried to use the flag WRITE_ONLY on a read only file.

Replay of test 4:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000004.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(22426813378560, 1, 65, 13) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0x1 fname='A' errno=13
```

### Ktest 5

```
ktest file : 'klee-out-11/test000005.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -r--r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x01\x02\x00\x00'
```

Failed because tried to use the flag WRITE_ONLY on a read only file. Flag combination: O_TRUNC (0x200) | O_WRONLY (0x1)??????

Replay of test 5:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000005.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(21608431616000, 513, 65, 13) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0x201 fname='A' errno=13
```

### Ktest 6

```
ktest file : 'klee-out-11/test000006.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: -r--r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'A\x00\x00\x00'
```

Failed because O_CREAT (0x40) + 0x1 (O_WRONLY) on read only.

Replay of ktest 6:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000006.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(16999435386880, 65, 65, 13) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0x41 fname='A' errno=13
```

### Ktest 7

```
ktest file : 'klee-out-11/test000007.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
object 0: hex : 0x00000000000000000000
object 0: text: ..........
object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: r--r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'A\x02\x00\x00'
```

Failed because of O_WRONLY | O_CREAT | O_TRUNC. Write only flag with read only permissions.

Replay of ktest7:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000007.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
fd:4294967295
errno:13
errno range:13
KLEE: WARNING ONCE: calling external: printf(11904907608064, 577, 65, 13) at full_symbolic_test_9_flags.c:43 9
open failed: flags=0x241 fname='A' errno=13
```

### Ktest 8

```
ktest file : 'klee-out-11/test000008.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: 'rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x00\x00\x00\x00'
```

First success with no flags.


Replay of ktest8:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000008.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
KLEE: WARNING ONCE: calling external: printf(12583320571904, 3, 0) at full_symbolic_test_9_flags.c:48 11
open succeeded: fd=3 flags=0x0
```

### Ktest 9

```
ktest file : 'klee-out-11/test000009.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'


object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x01\x02\x00\x00'
```

Another success with flags O_WRONLY and O_RDWR.

Replay ktest9:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000009.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
KLEE: WARNING ONCE: calling external: printf(10485211377664, 3, 513) at full_symbolic_test_9_flags.c:48 11
open succeeded: fd=3 flags=0x201
```

### Ktest 10

```
ktest file : 'klee-out-11/test000010.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'@\x00\x00\x00' //0x40000000
```

Success, basically open("A", O_CREAT).

Replay of ktest 10:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000010.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
KLEE: WARNING ONCE: calling external: printf(12101853745152, 3, 64) at full_symbolic_test_9_flags.c:48 11
open succeeded: fd=3 flags=0x40
```

### Ktest 11

```
ktest file : 'klee-out-11/test000011.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x03\x02\x00\x00'
```

Passed by basically doing: open("A", O_CREAT | O_TRUNC | O_RDWR). This combination is valid.

Replay of ktest 11:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000011.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x203
```

### Ktest 12

```
ktest file : 'klee-out-11/test000012.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--


object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x01\x00\x00\x00'
```

Very weird test, did this: open("A", O_WRONLY) and obviously it's valid.

Replay of ktest 12:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000012.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x1
```

### Ktest 13

```
ktest file : 'klee-out-11/test000013.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'\x02\x00\x00\x00'
```

Similar to above just asks for read and write only.

Replay of ktest 13:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000013.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x2
```

### Ktest 14

```
ktest file : 'klee-out-11/test000014.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'A\x00\x00\x00'
```

Did this basically: open("A", O_CREAT | O_WRONLY). Valid combination so passed.

Replay of ktest 14:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000014.ktest full_symbolic_test_9_flags.bc --sym-files 1 10
	
```
open succeeded: fd=3 flags=0x41
```

### Ktest 15

```
ktest file : 'klee-out-11/test000015.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']
num objects: 4
object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'B\x00\x00\x00' //0x42000000
```

Similar with the above: open("A", O_CREAT | O_RDWR). Valid combination so it passes.

Replay of ktest 15:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000015.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x42
```


### Ktest 16

```
ktest file : 'klee-out-11/test000016.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'A\x02\x00\x00'
```

Valid combination O_CREAT | O_RDWR | O_WRONLY??.

Replay of ktest 16:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000016.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x241
```

### Ktest 17

```
ktest file : 'klee-out-11/test000017.ktest'
args       : ['full_symbolic_test_9_flags.bc', '--sym-files', '1', '10']

object 0: name: 'A_data'
object 0: size: 10
object 0: data: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

object 1: name: 'A_data_stat'
object 1: size: 144
object 1: data: rw-r--r--

object 2: name: 'fname'
object 2: size: 2
object 2: data: b'A\xff'

object 3: name: 'flags'
object 3: size: 4
object 3: data: b'C\x02\x00\x00'
```

Final test with O_CREAT | O_TRUNC | O_RDWR | O_WRONLY. It seems O_RDWR overrides O_WRONLY so it passes.

Replay of ktest 17:

	klee --posix-runtime --libc=uclibc --replay-ktest-file=klee-out-11/test000017.ktest full_symbolic_test_9_flags.bc --sym-files 1 10

```
open succeeded: fd=3 flags=0x243
```
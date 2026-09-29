# A tool-independent file-system test suite for symbolic execution engines

127 tests covering eight POSIX system calls, written against a
tool-independent file-system API rather than against any one engine's
internals.

| System call | Tests | |
|---|---|---|
| `open` | 51 | |
| `close` | 10 | |
| `read` | 12 | |
| `write` | 13 | |
| `lseek` | 15 | |
| `chmod` | 13 | |
| `dup` / `dup2` | 13 | |
| **Total** | **127** | 102 pass, 25 fail |

Defects found are in `issues/klee_posix_findings.xlsx`. For what each test
does, see the spreadsheet in its `individual-tests/` folder. For example, the `open` folder has the `open_results` which explain each test.

## Requires the KLEE fork

> **This suite does not run on stock KLEE.**

The tests are written against file-system API primitives that stock KLEE does
not provide: `__file_create`, `file_exists`, `__assume`, `__gen_assert`,
`__file_offset`, `__is_sat`, `__is_certain` and others. They live in a fork:

* **[github.com/dino-fan777/klee](https://github.com/dino-fan777/klee)**, branch **`api_klee`**

## Quickest way to run them

The image builds the fork and clones this repository, so nothing needs to be
installed:

```bash
git clone https://github.com/dino-fan777/klee-tests.git
cd klee-tests
docker build -t klee-fsapi .
docker run -it --rm klee-fsapi
```

It opens in `/home/klee/klee-tests` and prints the usage notes on entry. Then:

```bash
make run_all            # all 127 tests, expect 102 passed / 25 failed
make run_open           # one system call
make run_open_12        # one test
```

`workflow.txt`, also printed when the container starts, covers how pass and
fail are decided, how to read a failure, and how to rebuild the engine after
editing the fork.

### Pinning exact revisions

By default the image is built from the tip of the fork's `api_klee` branch
and of this repository's `main`. That means two builds run on different days
can produce different images, because either branch may have moved in
between.

To get a build that can be reproduced exactly, name the commits instead:

```bash
docker build \
  --build-arg FORK_REF=40b3b74109094e5930ee06eb9f948f02dbfc8e01 \
  --build-arg TESTS_REF=4788115fa49ed56ea52ac5979e4a0eaa86184d6b \
  -t klee-fsapi .
```

`FORK_REF` selects the revision of the fork, `TESTS_REF` the revision of this
repository. Both accept a branch, a tag, or a commit hash, but a commit hash
**must be given in full**: git cannot fetch an abbreviated one over the
network.

Worth doing when reporting results, so the numbers can be tied to the exact
engine and tests that produced them.

## Running without Docker

You need the fork built and its `klee` binary on your `PATH`, plus `clang`
matching the LLVM that KLEE was built against. Then:

```bash
make run_all
```

Each suite compiles with `-I../../include`, so `include/test_helper.h` is
found automatically from `individual-tests/<syscall>/`.

## Layout

```
include/            test_helper.h, the assertions and symbolic-input helpers
individual-tests/   one folder per system call, each with its results spreadsheet
issues/             klee_posix_findings.xlsx, the defects the suite found
docker/             rebuild_posix.sh and rebuild_all.sh, used by the image
Dockerfile          builds the fork and this suite into a ready-to-run image
workflow.txt        how to run the tests, and how to read the results
```

## How a test is judged

A test **fails** when KLEE reports:

```
KLEE: done: completed paths = 0
```

No input satisfying the test's `__assume()` constraints also satisfies its
`__gen_assert()` assertions, so the behaviour it describes is unreachable.
Anything else passes.

This is deliberately not "did any path hit an assertion". With a symbolic
file, the engine legitimately explores permission configurations in which the
operation under test is supposed to be refused, so an assertion failing on
*some* path is expected and is not by itself a defect.

## What a test looks like

```c
#include "test_helper.h"

int main(void) {
   char fname[2];
   int  flags;

   declare_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   cleanup_fd(__file_create("A_data"));     // the test creates its own file
   __assume(file_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags);

   __gen_assert(open_succeeds(fd));
   __gen_assert(fd_is(fd, 3));

   __gen_assert(close_succeeds(close(fd)));
   return 0;
}
```

# A tool-independent file-system test suite for symbolic execution engines

127 tests covering eight POSIX system calls, written against a
tool-independent file-system API rather than against any one engine's
internals. Running them against KLEE's POSIX summaries found six defects,
four of which have since been fixed upstream.

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

The 25 failures are **defects in the engine, not broken tests**. See
`issues/klee_posix_findings.xlsx`, and the per-system-call spreadsheet in
each `individual-tests/` folder for what every test does.

## Requires the KLEE fork

> **This suite does not run on stock KLEE.**

The tests are written against file-system API primitives that stock KLEE does
not provide: `__file_create`, `file_exists`, `__assume`, `__gen_assert`,
`__file_offset`, `__is_sat`, `__is_certain` and others. They live in a fork:

* **[github.com/dino-fan777/klee](https://github.com/dino-fan777/klee)**, branch **`api_klee`**

`__file_create` is what makes the suite tool-independent. Each test creates
the file it operates on, so KLEE's `--sym-files` command-line option is not
used anywhere. On stock KLEE the tests do not merely fail, they do not
compile.

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

To pin exact revisions rather than branch tips:

```bash
docker build --build-arg FORK_REF=<commit> --build-arg TESTS_REF=<commit> -t klee-fsapi .
```

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

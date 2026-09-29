# Tool-independent file-system test suite for symbolic execution engines,
# running against a KLEE fork that implements the file-system API.
#
# Build:  docker build -t klee-fsapi .
# Run:    docker run -it --rm klee-fsapi
#
# The suite does NOT work on stock KLEE. It needs the API primitives
# (__file_create, file_exists, __assume, __gen_assert, ...) that the fork
# adds to KLEE's POSIX runtime, so the image builds that fork from source.

FROM klee/klee:3.1

# Which revisions to build. Override to pin an exact commit, for example:
#   docker build --build-arg FORK_REF=40b3b74 -t klee-fsapi .
ARG FORK_REF=api_klee
ARG TESTS_REF=main
ARG FORK_REPO=https://github.com/dino-fan777/klee.git
ARG TESTS_REPO=https://github.com/dino-fan777/klee-tests.git

# git is not in the base image. The kitware apt source ships an expired key,
# which makes "apt-get update" report an error while still refreshing the
# Ubuntu repositories that actually matter, hence the "|| true".
USER root
RUN apt-get update -qq || true \
 && apt-get install -y --no-install-recommends git \
 && rm -rf /var/lib/apt/lists/*

USER klee
WORKDIR /home/klee

# Fetched rather than cloned, because "git clone --branch" only accepts a
# branch or tag, so it cannot pin a commit. Fetching the ref directly works
# for branches, tags and full 40-character commit hashes alike. Abbreviated
# hashes are not accepted by the protocol, so FORK_REF and TESTS_REF must be
# given in full when pinning a commit.
RUN mkdir klee_fork klee-tests \
 && cd /home/klee/klee_fork \
 && git init -q && git remote add origin "${FORK_REPO}" \
 && git fetch -q --depth 1 origin "${FORK_REF}" && git checkout -q FETCH_HEAD \
 && cd /home/klee/klee-tests \
 && git init -q && git remote add origin "${TESTS_REPO}" \
 && git fetch -q --depth 1 origin "${TESTS_REF}" && git checkout -q FETCH_HEAD

COPY --chown=klee:klee docker/rebuild_posix.sh docker/rebuild_all.sh /home/klee/
COPY --chown=klee:klee workflow.txt /home/klee/workflow.txt

# Build the fork into the base image's existing KLEE tree. rebuild_all.sh is
# used rather than rebuild_posix.sh because the fork also changes the core
# C++: klee_is_sat and klee_is_certain live in SpecialFunctionHandler and are
# compiled into the klee binary, not into the runtime bitcode.
RUN chmod +x /home/klee/rebuild_posix.sh /home/klee/rebuild_all.sh \
 && /home/klee/rebuild_all.sh

ENV PATH=/home/klee/klee_build/bin:$PATH
WORKDIR /home/klee/klee-tests

# Greet with the instructions rather than a bare prompt.
CMD ["/bin/bash", "-lc", "cat /home/klee/workflow.txt; exec bash"]

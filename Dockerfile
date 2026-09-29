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

RUN git clone --branch "${FORK_REF}" --depth 1 "${FORK_REPO}" klee_fork \
 && git clone --branch "${TESTS_REF}" --depth 1 "${TESTS_REPO}" klee-tests

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

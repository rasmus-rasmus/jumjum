FROM ubuntu:24.04

SHELL ["/bin/bash", "-c"]

RUN apt-get update \
    && apt-get -y --no-install-recommends install cmake build-essential clang libglm-dev \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /home/jumjum
COPY src/ src/
COPY scripts/build.sh scripts/build.sh
COPY CMakeLists.txt .

RUN ./scripts/build.sh

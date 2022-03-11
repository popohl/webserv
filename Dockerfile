FROM debian:buster

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y libgtest-dev cmake clang
WORKDIR /code
ENTRYPOINT cd srcs/testing && cmake -S . -B build && cmake --build build && cd build && ctest

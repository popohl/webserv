FROM debian:buster

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y libgtest-dev cmake clang
WORKDIR /code
ENTRYPOINT make

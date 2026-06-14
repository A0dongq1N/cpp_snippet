FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc \
    procps \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY bad_init.c zombie_factory.c monitor.sh ./
RUN gcc -Wall -o bad_init bad_init.c \
 && gcc -Wall -o zombie_factory zombie_factory.c \
 && chmod +x monitor.sh

# Scene 1: bad_init 作为 PID 1，永不 waitpid()
# zombie_factory 退出后，孤儿被 bad_init 收养但无法 reap → 僵尸堆积
ENTRYPOINT ["/app/bad_init"]

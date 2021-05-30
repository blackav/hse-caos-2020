CC = gcc
CFLAGS = -O2 -Wall -std=gnu11 -Werror -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -m32 -pthread

all : gthr gthrg atomic2 atomic4

gthr : gthr.c gthr2.S

gthrg : gthrg.c gthr2.S

atomic2 : atomic2.c

atomic4 : atomic4.c


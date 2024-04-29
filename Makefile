CC = gcc
EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer
CFLAGS = -ffreestanding -fno-stack-protector -nostdlib -nostdinc -static -m32 -Wall -g -O2
SOURCES_NSD = prod-cons.c

hexconv: hexconv.c
	$(CC) $(CFLAGS) -o $@ $<

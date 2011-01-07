.PHONY: all

CC = gcc
CFLAGS = -g -O2 -Wall

COMPILE = $(CC) $(CFLAGS)

all: vmcptxt vmclockd

vmcptxt: vmcptxt.o vmw_door.o
	$(COMPILE) -o $@ $^

vmclockd: vmclockd.o vmw_door.o
	$(COMPILE) -o $@ $^ -lrt

vmcptxt.o vmclockd.o vmw_door.o: vmw_door.h

.c.o:
	$(COMPILE) -c $<

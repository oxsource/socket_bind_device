CC       = gcc
CFLAGS   = -Wall -O2 -fPIC
LDFLAGS  =
LDLIBS   = -ldl

MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
SRCDIR   = $(MAKEFILE_DIR)/src
OUTDIR   = $(MAKEFILE_DIR)/out
LIBDIR   = $(MAKEFILE_DIR)/../lib

SRC_LIB  := $(SRCDIR)/socket_iface.c
SRC_BIN  := $(SRCDIR)/socket.c $(SRC_LIB)

BIN_NAME = socket
LIB_NAME = libsocketiface.so

TARGET_BIN = $(OUTDIR)/$(BIN_NAME)
TARGET_LIB = $(OUTDIR)/$(LIB_NAME)

.PHONY: all clean install

all: $(TARGET_LIB) $(TARGET_BIN)

$(TARGET_LIB): $(OUTDIR) $(SRC_LIB)
	$(CC) $(CFLAGS) -shared -o $@ $(SRC_LIB) $(LDFLAGS) $(LDLIBS)

$(TARGET_BIN): $(OUTDIR) $(SRC_BIN)
	$(CC) $(CFLAGS) -o $@ $(SRC_BIN) $(LDFLAGS) $(LDLIBS)

$(OUTDIR):
	mkdir -p $@

install: $(TARGET_LIB) | $(LIBDIR)
	cp -f $< $(LIBDIR)/

$(LIBDIR):
	mkdir -p $@

clean:
	rm -rf $(OUTDIR)


CC       = gcc
CFLAGS   = -Wall -O2 -fPIC
LDFLAGS  =

SRCDIR   = src
OUTDIR   = out
LIBDIR   = ../lib

SRC_LIB  := $(SRCDIR)/iface.c
SRC_BIN  := $(SRCDIR)/dns.c $(SRC_LIB)

BIN_NAME = dns
LIB_NAME = libdnsiface.so

TARGET_BIN = $(OUTDIR)/$(BIN_NAME)
TARGET_LIB = $(OUTDIR)/$(LIB_NAME)

.PHONY: all clean install

all: $(TARGET_LIB) $(TARGET_BIN)

$(TARGET_LIB): $(OUTDIR) $(SRC_LIB)
	$(CC) $(CFLAGS) -shared -o $@ $(SRC_LIB) $(LDFLAGS)

$(TARGET_BIN): $(OUTDIR) $(SRC_BIN)
	$(CC) $(CFLAGS) -o $@ $(SRC_BIN) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $@

install: $(TARGET_LIB) | $(LIBDIR)
	cp -f $< $(LIBDIR)/

$(LIBDIR):
	mkdir -p $@

clean:
	rm -rf $(OUTDIR)

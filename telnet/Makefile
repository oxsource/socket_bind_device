CC       = gcc
CFLAGS   = -Wall -O2 -fPIC
LDFLAGS  = -L$(LIBDIR) -ldnsiface -lsocketiface

SRCDIR   = src
OUTDIR   = out
LIBDIR   = ../lib

SRC_BIN  := $(SRCDIR)/telnet.c

BIN_NAME = telnet

TARGET_BIN = $(OUTDIR)/$(BIN_NAME)

.PHONY: all clean

all: $(TARGET_BIN)

$(TARGET_BIN): $(OUTDIR) $(SRC_BIN)
	$(CC) $(CFLAGS) -o $@ $(SRC_BIN) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $@

clean:
	rm -rf $(OUTDIR)

run: all
	LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH \
	$(TARGET_BIN) $(ARGS)


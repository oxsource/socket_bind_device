CC       = gcc
CFLAGS   = -Wall -O2 -fPIC
LDFLAGS  = -L$(LDPATH) -ldnsiface -lsocketiface -Wl,-rpath=$(LDPATH)

SRCDIR   = src
OUTDIR   = out
LIBDIR   = ../lib
LDPATH   = $(LIBDIR)

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


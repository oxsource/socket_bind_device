CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =

SRCDIR = src
OUTDIR = out

DNS_SRC = $(SRCDIR)/dns.c
IFACE_SRC = $(SRCDIR)/iface.c
IFACE_HDR = $(SRCDIR)/iface.h

TARGET_EXE = $(OUTDIR)/dns
TARGET_SO  = $(OUTDIR)/libdnsiface.so

LIBDIR = ../lib

.PHONY: all clean

all: $(TARGET_SO) $(TARGET_EXE)

$(TARGET_SO): $(IFACE_SRC) $(IFACE_HDR) | $(OUTDIR)
	$(CC) -fPIC -shared -o $@ $<

$(TARGET_EXE): $(DNS_SRC) $(IFACE_SRC) $(IFACE_HDR) | $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $(DNS_SRC) $(IFACE_SRC) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(OUTDIR)

install: $(TARGET_SO) | $(LIBDIR)
	cp -f $(TARGET_SO) $(LIBDIR)/

$(LIBDIR):
	mkdir -p $(LIBDIR)

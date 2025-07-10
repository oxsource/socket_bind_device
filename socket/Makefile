CC = gcc
CFLAGS = -Wall -O2 -fPIC
LDFLAGS = -shared
TARGET = libsocketdevice.so
SRC = device.c
OUTDIR = out
LIBDIR = ../lib

all: $(OUTDIR)/$(TARGET)

$(OUTDIR)/$(TARGET): $(SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install: $(OUTDIR)/$(TARGET) | $(LIBDIR)
	cp -f $< $(LIBDIR)/

$(LIBDIR):
	mkdir -p $(LIBDIR)

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean install

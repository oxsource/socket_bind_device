CC = gcc
CFLAGS = -Wall -O2 -fPIC
LDFLAGS = -shared
OUTDIR = out
LIBDIR = ../lib

LIB_SRC = src/socket_iface.c
LIB_HDR = src/socket_iface.h
LIB_TARGET = libsocketiface.so

APP_SRC = src/socket.c
APP_TARGET = socket

all: $(OUTDIR)/$(LIB_TARGET) $(OUTDIR)/$(APP_TARGET)

$(OUTDIR)/$(LIB_TARGET): $(LIB_SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OUTDIR)/$(APP_TARGET): $(APP_SRC) $(OUTDIR)/$(LIB_TARGET)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $< -L$(OUTDIR) -lsocketiface -Wl,-rpath=$(OUTDIR)

install: $(OUTDIR)/$(LIB_TARGET) | $(LIBDIR)
	cp -f $< $(LIBDIR)/

$(LIBDIR):
	mkdir -p $(LIBDIR)

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean install

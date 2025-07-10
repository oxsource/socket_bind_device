CC = gcc
CFLAGS = -Wall -O2
LIBDIR = ../lib
LDFLAGS = -L$(LIBDIR) -ldnsiface

OUTDIR = out
TARGET = telnet
SRC = telnet.c

.PHONY: all clean run

all: $(OUTDIR)/$(TARGET)

$(OUTDIR)/$(TARGET): $(SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: all
	LD_PRELOAD=$(LIBDIR)/libsocketdevice.so \
	LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH \
	$(OUTDIR)/$(TARGET) $(ARGS)

clean:
	rm -rf $(OUTDIR)/${TARGET}

UNAME_S := $(shell uname -s)
CC       = gcc
CFLAGS   = -Wall -O2 -fPIC
ifeq ($(UNAME_S),Linux)
	LDFLAGS  = -L$(LDPATH) -ldnsiface -lsocketiface -Wl,-rpath=$(LDPATH) -lcurl
	DUMP_DEP = ldd $(TARGET_BIN)
else ifeq ($(UNAME_S),Darwin)
	LDFLAGS  = -L$(LDPATH) -ldnsiface -lsocketiface -Wl,-rpath,$(LDPATH) -lcurl
	DUMP_DEP = otool -L $(TARGET_BIN)
endif

MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
SRCDIR   = $(MAKEFILE_DIR)/src
OUTDIR   = $(MAKEFILE_DIR)/out
LIBDIR   = $(MAKEFILE_DIR)/../lib
LDPATH   = $(LIBDIR)

SRC_BIN  := $(SRCDIR)/curl.c

BIN_NAME = curl

TARGET_BIN = $(OUTDIR)/$(BIN_NAME)

.PHONY: all clean dep

all: $(TARGET_BIN)

$(TARGET_BIN): $(OUTDIR) $(SRC_BIN)
	$(CC) $(CFLAGS) -o $@ $(SRC_BIN) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $@

clean:
	rm -rf $(OUTDIR)

dep: $(TARGET_BIN)
	@echo ">> Shared library dependencies for $(TARGET_BIN):"
	@$(DUMP_DEP)

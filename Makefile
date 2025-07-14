SUBDIRS := dns socket telnet curl

.PHONY: all clean install $(SUBDIRS)

all: dns socket telnet curl

dns socket:
	@echo "==== Building $@ ===="
	@$(MAKE) -C $@

install: dns socket
	@for dir in dns socket; do \
		echo "==== Installing $$dir ===="; \
		$(MAKE) -C $$dir install; \
	done

telnet: install
	@echo "==== Building telnet ===="
	@$(MAKE) -C $@

curl: install
	@echo "==== Building curl ===="
	@$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		echo "==== Cleaning $$dir ===="; \
		$(MAKE) -C $$dir clean; \
	done
	@rm -rf lib

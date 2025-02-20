


SUBDIRS = $(shell find . -type f -name "Makefile" -path "*/_Tests/*" -exec dirname {} \;)

ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
OBJ_PATH				=		$(ROOT_PATH)/_build/debug 

# Default target
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir --quiet || exit 1; \
	done
	@echo "All compiled"
# Run tests in all subdirectories containing a Makefile
test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir test --quiet || exit 1; \
	done

debug: test

# Clean all subdirectories
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir fclean --quiet; \
	done
	@echo "All clean"

fclean:	
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir fclean --quiet; \
	done
	@rm -rf $(OBJ_PATH)
	@echo "All fclean"

.PHONY: all run_subdirs clean
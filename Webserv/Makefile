

MAKEFILE_NAME 				= 		Makefile
TEST_FOLDERNAME 			= 		_Tests
INTEGRATION_FOLDERNAME 		= 		_IntegrationTests
TEST_COMMAND 				= 		test
DEBUG_COMMAND 				= 		debug

ROOT_PATH					=		$(shell git rev-parse --show-toplevel)
TEST_PATH 					= 		$(ROOT_PATH)/Webserv

SUBDIRS = $(shell find $(TEST_PATH) -type f \( -path "*/$(TEST_FOLDERNAME)/*" -o -path "*/$(INTEGRATION_FOLDERNAME)/*" \) -name "$(MAKEFILE_NAME)" -exec dirname {} \;)


# Default target
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir --quiet || exit 1; \
	done
	@echo "All compiled"
# Run tests in all subdirectories containing a Makefile
test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(TEST_COMMAND) --quiet || exit 1; \
	done

debug:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(DEBUG_COMMAND) --quiet || exit 1; \
	done
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
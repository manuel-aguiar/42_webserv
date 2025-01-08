# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/23 10:33:17 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/27 18:48:44 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SUBDIRS = $(shell find . -type f -name "Makefile" -path "*/_Tests/*" -exec dirname {} \;)

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
	@rm -rf _build
	@echo "All fclean"

.PHONY: all run_subdirs clean
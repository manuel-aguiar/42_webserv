# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/23 10:33:17 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/23 10:46:35 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SUBDIRS = $(shell find . -type f -name "Makefile" ! -path "./Makefile" -exec dirname {} \;)

# Default target
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done
# Run tests in all subdirectories containing a Makefile
run:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir run || exit 1; \
	done

# Clean all subdirectories
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir fclean; \
	done

fclean: clean
	rm -rf _build

.PHONY: all run_subdirs clean
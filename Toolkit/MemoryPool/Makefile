# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 14:13:01 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/27 19:03:33 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
# Makefile whose purpose is to compile the various memory pools into a library
# In reality, it's all templates (header only) with the exception of the
# Nginx memory pool, which is concrete and static
#

## GENERAL RULES

NAME 				= 	libmemorypool.a
COMP 				= 	c++ -std=c++98
FLAGS 				= 	-Wall -Wextra -Werror -MMD -MP
LIB 				= 	ar rcs
RM					=	rm
MAKE				=	make

DEBUG_FLAGS 		= 	-g #-fsanitize=address
BUILD_FLAGS 		= 	-DNDEBUG

## FOLDERS

# Paths
ROOT_PATH				=		../../Toolkit
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

					

SRCS				=	$(SRC_PATH)/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp			\
						$(SRC_PATH)/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp			

OBJS 				=	$(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(SRCS))

DEPS 				= 	$(OBJS:.o=.d)

BUILD 							= 
									ifeq ($(MAKECMDGOALS),debug)
										BUILD = $(DEBUG_FLAGS)
									else
										BUILD = $(BUILD_FLAGS)
									endif

## Compile

$(NAME): $(OBJS)
	$(LIB) $@ $(OBJS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(COMP) $(FLAGS) $(BUILD) -c $< -o $@

-include $(DEPS)

## Rules

all: $(NAME)

debug: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	@if [ -n "$(wildcard  $(NAME))" ]; then \
        $(RM) $(NAME); \
    fi

run:

re: fclean all

git: fclean
	git add * && git commit -m "commiting" && git push

.PHONY: clean fclean re git debug run


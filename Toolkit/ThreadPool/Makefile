# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/06 11:51:09 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/26 11:27:29 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile to compile the threadpool into a library
# The end user will be responsible for linking pthread (-lpthread) when compiling
# the threadpoll into their own projects


## GENERAL RULES
NAME 				= 	libthreadpool.a
COMP 				= 	c++ -std=c++98
FLAGS 				= 	-Wall -Wextra -Werror -MMD -MP
LIB 				= 	ar rcs
RM					=	rm
MAKE				=	make

DEBUG_FLAGS 		= 	-g #-fsanitize=address
BUILD_FLAGS 		= 	-DNDEBUG 

## FOLDERS

OBJ_PATH			=	_build
SRC_PATH			=	.

SRCS				=	$(SRC_PATH)/ThreadPool/ThreadPool.cpp					\
						$(SRC_PATH)/ThreadWorker/ThreadWorker.cpp				\
						$(SRC_PATH)/TaskQueue/TaskQueue.cpp							


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
	@if [ -d "$(OBJ_PATH)" ]; then \
        rm -rf $(OBJ_PATH); \
    fi

fclean: clean
	@if [ -n "$(wildcard  $(NAME))" ]; then \
        $(RM) $(NAME); \
    fi

run:

re: fclean all

git: fclean
	git add * && git commit -m "commiting" && git push

.PHONY: clean fclean re git debug run


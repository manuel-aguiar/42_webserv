# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 14:13:01 by mmaria-d          #+#    #+#              #
#    Updated: 2024/09/27 14:26:23 by mmaria-d         ###   ########.fr        #
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
FLAGS 				= 	-Wall -Wextra -Werror
LIB 				= 	ar rcs
RM					=	rm
MAKE				=	make


## FOLDERS

INC_PATH			=	include
SRC_PATH			=	source
OBJ_PATH			=	objs


## Add Paths

ONLYALLOC_PATH		=	Alloc
DEALLOC_PATH        =	Dealloc
ALIGNDEALLOC_PATH	=	AlignDealloc
NGINXMEMPOOL_PATH	=	Nginx_MemoryPool



INCS				=	MemoryPool.h	

OTHER_INCS			= 	$(ALIGNDEALLOC_PATH)/MemoryPool_AlignDealloc.hpp			\
						$(ALIGNDEALLOC_PATH)/MemoryPool_AlignDealloc.tpp			\
						$(ALIGNDEALLOC_PATH)/SharedMPool_AlignDealloc.hpp			\
						$(ALIGNDEALLOC_PATH)/SharedMPool_AlignDealloc.tpp			\
						\
						$(DEALLOC_PATH)/MemoryPool_Dealloc.hpp						\
						$(DEALLOC_PATH)/MemoryPool_Dealloc.tpp						\
						$(DEALLOC_PATH)/SharedMPool_Dealloc.hpp						\
						$(DEALLOC_PATH)/SharedMPool_Dealloc.tpp						\
						\
						$(ONLYALLOC_PATH)/MemoryPool_Alloc.hpp						\
						$(ONLYALLOC_PATH)/MemoryPool_Alloc.tpp						\
						$(ONLYALLOC_PATH)/SharedMPool_Alloc.hpp						\
						$(ONLYALLOC_PATH)/SharedMPool_Alloc.tpp						\
						\
						$(NGINXMEMPOOL_PATH)/Nginx_MemoryPool.hpp						

SRCS				=	$(NGINXMEMPOOL_PATH)/Nginx_MemoryPool.cpp			\
						$(NGINXMEMPOOL_PATH)/Nginx_MPool_Block.cpp	

OBJS 				:=	$(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(SRCS))



## Compile

$(NAME): $(OBJS)
	$(LIB) $@ $(OBJS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(INCS) $(OTHER_INCS)
	@mkdir -p $(dir $@)
	$(COMP) $(FLAGS) -I$(INC_PATH) -c $< -o $@


## Rules

all: $(NAME)


clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
        rm -rf $(OBJ_PATH); \
    fi

fclean: clean
	@if [ -n "$(wildcard  $(NAME))" ]; then \
        $(RM) $(NAME); \
    fi

re: fclean all

git: fclean
	git add * && git commit -m "commiting" && git push

.PHONY: clean fclean re git


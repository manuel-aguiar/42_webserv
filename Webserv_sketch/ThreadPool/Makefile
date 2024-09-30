# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/06 11:51:09 by mmaria-d          #+#    #+#              #
#    Updated: 2024/09/30 09:41:00 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile to compile the threadpool into a library
# The end user will be responsible for linking pthread (-lpthread) when compiling
# the threadpoll into their own projects


## GENERAL RULES
NAME 				= 	libthreadpool.a
COMP 				= 	c++ -std=c++98
FLAGS 				= 	-Wall -Wextra -Werror
LIB 				= 	ar rcs
RM					=	rm
MAKE				=	make

DEBUG_FLAGS 		= 	-g
BUILD_FLAGS 		= 	-DNDEBUG 

## FOLDERS

INC_PATH			=	include
SRC_PATH			=	source
OBJ_PATH			=	objs

INC_ABST_PATH		= 	$(addprefix $(INC_PATH)/, Abstract)
INC_CONC_PATH		=	$(addprefix $(INC_PATH)/, Concrete)

SRC_ABST_PATH		= 	$(addprefix $(SRC_PATH)/, Abstract)
SRC_CONC_PATH		=	$(addprefix $(SRC_PATH)/, Concrete)




## FILES
INC_ABST_FILES		=	AThread.hpp											\
						IThreadPool.hpp										\
						IThreadTask.hpp										\
						IThreadTaskQueue.hpp

INC_CONC_FILES		=	ThreadPool.hpp										\
						ThreadPoolWorker.hpp								\
						ThreadTaskQueue.hpp									\
						ThreadTask.tpp										\
						ThreadTask_Memb_Args_NoReturn.tpp					\
						ThreadTask_Memb_Args_NoReturn_Const.tpp				\
						ThreadTask_Memb_Args_Return.tpp						\
						ThreadTask_Memb_Args_Return_Const.tpp				\
						ThreadTask_NonMemb_Args_Return.tpp					\
						ThreadTask_NonMemb_Args_NoReturn.tpp

SRC_ABST_FILES		=	AThread.cpp

SRC_CONC_FILES		=	ThreadPool.cpp										\
						ThreadPoolWorker.cpp								\
						ThreadTaskQueue.cpp									


## Add Paths


INCS				=	$(addprefix $(INC_ABST_PATH)/, $(INC_ABST_FILES))	\
						$(addprefix $(INC_CONC_PATH)/, $(INC_CONC_FILES))	

SRCS				=	$(addprefix $(SRC_ABST_PATH)/, $(SRC_ABST_FILES))	\
						$(addprefix $(SRC_CONC_PATH)/, $(SRC_CONC_FILES))	

OBJS 				:=	$(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(SRCS))

BUILD 							= 
									ifeq ($(MAKECMDGOALS),debug)
										BUILD = $(DEBUG_FLAGS)
									else
										BUILD = $(BUILD_FLAGS)
									endif

## Compile

$(NAME): $(OBJS)
	$(LIB) $@ $(OBJS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(INCS)
	@mkdir -p $(dir $@)
	$(COMP) $(FLAGS) $(BUILD) -I$(INC_PATH) -c $< -o $@
	


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

re: fclean all

git: fclean
	git add * && git commit -m "commiting" && git push

.PHONY: clean fclean re git debug


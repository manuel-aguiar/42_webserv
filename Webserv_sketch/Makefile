# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 15:59:23 by mmaria-d          #+#    #+#              #
#    Updated: 2024/09/30 08:54:00 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 							= 	webserv		
COMP 							= 	c++ -std=c++98
FLAGS 							= 	-Wall -Wextra -Werror -g
BUILD_FLAGS 					= 	-DNDEBUG

THREADPOOL_PATH 				= 	ThreadPool
THREADPOOL_LIB					= 	libthreadpool.a

MEMORYPOOL_PATH					= 	MemoryPool
MEMORYPOOL_LIB					= 	libmemorypool.a

RM 								= 	rm -rf
MAKE 							= 	make

SRC_PATH = .
OBJ_PATH = objs

SRCS = 							$(SRC_PATH)/main.cpp										\
								$(SRC_PATH)/Server/Server.cpp								\
								$(SRC_PATH)/ListeningSocket/ListeningSocket.cpp			\
								$(SRC_PATH)/CgiHandler/CgiHandler.cpp						\
								$(SRC_PATH)/Connection/Connection.cpp						\
								$(SRC_PATH)/Connection/ConnectionPool.cpp					\
								$(SRC_PATH)/Event/Event.cpp								\
								$(SRC_PATH)/Request/Request.cpp										



THREADPOOL_LINKER				= 	-l$(patsubst lib%.a,%,$(THREADPOOL_LIB))
MEMORYPOOL_LINKER 				= 	-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

LIB_THREADPOOL 					= 	-L./$(THREADPOOL_PATH) $(THREADPOOL_LINKER)
LIB_MEMORYPOOL 					= 	-L./$(MEMORYPOOL_PATH) $(MEMORYPOOL_LINKER)


LIBS 							= 	$(LIB_THREADPOOL) $(LIB_MEMORYPOOL)


OBJS 							= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))


BUILD 							= 
									ifeq ($(MAKECMDGOALS),debug)
										BUILD = 
									else
										BUILD = $(BUILD_FLAGS)
									endif

all: 	$(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C $(THREADPOOL_PATH)
	@$(MAKE) -C $(MEMORYPOOL_PATH)
	$(COMP) $(FLAGS) $(BUILD) $(LIBS) -o $(NAME) main.cpp

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(COMP) $(FLAGS) $(BUILD) -c $< -o $@

debug: 	$(NAME)

clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
        rm -rf $(OBJ_PATH); \
    fi
	@$(MAKE) clean -C $(THREADPOOL_PATH)
	@$(MAKE) clean -C $(MEMORYPOOL_PATH)
	
fclean: clean
	@if [ -n "$(wildcard  $(NAME))" ]; then \
        $(RM) $(NAME); \
    fi
	@if [ -n "$(wildcard  $(THREADPOOL_PATH)/$(THREADPOOL_LIB))" ]; then \
        $(RM) $(THREADPOOL_PATH)/$(THREADPOOL_LIB); \
    fi
	@if [ -n "$(wildcard  $(MEMORYPOOL_PATH)/$(MEMORYPOOL_LIB))" ]; then \
        $(RM) $(MEMORYPOOL_PATH)/$(MEMORYPOOL_LIB); \
    fi

re: fclean all

git:
	git add .
	git commit -m "update"
	git push

.PHONY: debug clean fclean re git
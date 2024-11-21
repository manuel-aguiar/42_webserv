# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 15:59:23 by mmaria-d          #+#    #+#              #
#    Updated: 2024/11/21 12:13:25 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 							= 	webserv
COMP 							= 	c++ -std=c++98
FLAGS 							= 	-Wall -Wextra -Werror
EXTERN_LINKER 					= 	-lpthread
DEBUG_FLAGS 					= 	-g #-fsanitize=address
BUILD_FLAGS 					= 	-DNDEBUG

THREADPOOL_PATH 				= 	../Toolkit/ThreadPool
THREADPOOL_LIB					= 	libthreadpool.a

MEMORYPOOL_PATH					= 	../Toolkit/MemoryPool
MEMORYPOOL_LIB					= 	libmemorypool.a

RM 								= 	rm -rf
MAKE 							= 	make

SRC_PATH 						= 	.
OBJ_PATH 						= 	_objs

SRCS 							=		$(SRC_PATH)/main.cpp										\
										$(SRC_PATH)/ListeningSocket/ListeningSocket.cpp				\
										$(SRC_PATH)/CgiManager/CgiManager.cpp						\
										$(SRC_PATH)/CgiManager/python-cgi/pythonCgi.cpp				\
										$(SRC_PATH)/CgiManager/CgiLiveRequest/CgiLiveRequest.cpp	\
										$(SRC_PATH)/Connection/Connection.cpp						\
										$(SRC_PATH)/Connection/ConnectionPool.cpp					\
										$(SRC_PATH)/Event/Event.cpp									\
										$(SRC_PATH)/Event/HandlerFunction.cpp						\
										$(SRC_PATH)/Server/Server.cpp								\
										$(SRC_PATH)/EventManager/EventManager.cpp					\
										$(SRC_PATH)/Globals/Globals.cpp								\
										$(SRC_PATH)/Clock/Clock.cpp									\
										$(SRC_PATH)/SignalHandler/SignalHandler.cpp					\
										$(SRC_PATH)/FileDescriptor/FileDescriptor.cpp				\
										$(SRC_PATH)/Logs/LogFile.cpp								\
										$(SRC_PATH)/Logs/LogFileThreadSafe.cpp						\
										$(SRC_PATH)/Logs/LogStream.cpp								\
										$(SRC_PATH)/Logs/LogStreamThreadSafe.cpp


THREADPOOL_LINKER				= 	-l$(patsubst lib%.a,%,$(THREADPOOL_LIB))
MEMORYPOOL_LINKER 				= 	-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

LIB_THREADPOOL 					= 	-L./$(THREADPOOL_PATH) $(THREADPOOL_LINKER)
LIB_MEMORYPOOL 					= 	-L./$(MEMORYPOOL_PATH) $(MEMORYPOOL_LINKER)


LIBS 							= 	$(LIB_THREADPOOL) $(LIB_MEMORYPOOL) $(EXTERN_LINKER)


OBJS 							= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))


BUILD 							=
									ifeq ($(MAKECMDGOALS),debug)
										BUILD = $(DEBUG_FLAGS)
									else
										BUILD = $(BUILD_FLAGS)
									endif



$(NAME): OBJS
	@$(MAKE) $(MAKECMDGOALS) -C $(THREADPOOL_PATH)
	@$(MAKE) $(MAKECMDGOALS) -C $(MEMORYPOOL_PATH)

	$(COMP) $(FLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(COMP) $(FLAGS) $(BUILD) -c $< -o $@

all: $(NAME)

debug: $(NAME)

objs: $(OBJS)

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

run: $(NAME)
	./$(NAME)

re: fclean all

git: fclean
	git add .
	git commit -m "update"
	git push

.PHONY: debug clean fclean objs re git run

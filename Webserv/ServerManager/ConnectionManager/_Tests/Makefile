# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/22 10:10:58 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/22 10:13:17 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

ROOT_PATH = ../../../..

EXTERN_LINKER 					= 	-lpthread

THREADPOOL_PATH 				= 	$(ROOT_PATH)/Toolkit/ThreadPool
THREADPOOL_LIB					= 	libthreadpool.a

MEMORYPOOL_PATH					= 	$(ROOT_PATH)/Toolkit/MemoryPool
MEMORYPOOL_LIB					= 	libmemorypool.a

# Paths

SRC_PATH = $(ROOT_PATH)/Webserv/.
OBJ_PATH = $(ROOT_PATH)/Webserv/_build


SRCS =  $(SRC_PATH)/ServerManager/ConnectionManager/_Tests/test.cpp \
											\
		$(SRC_PATH)/ServerManager/ConnectionManager/ConnectionManager.cpp \
		$(SRC_PATH)/ServerManager/ConnectionManager/ManagedConnection.cpp \
										\
		$(SRC_PATH)/Globals/_Mock/MockLogFile.cpp \
		$(SRC_PATH)/Globals/_Mock/MockGlobals.cpp \
		$(SRC_PATH)/Globals/_Mock/MockClock.cpp \
											\
		$(SRC_PATH)/Globals/SignalHandler/SignalHandler.cpp \
											\
		$(SRC_PATH)/Connection/_Mock/MockConnection.cpp \
		\
		$(SRC_PATH)/GenericUtils/FileDescriptor/FileDescriptor.cpp \
		$(SRC_PATH)/Event/Event.cpp \
		$(SRC_PATH)/Event/EventGetSetAccess.cpp \
		$(SRC_PATH)/Callback/Callback.cpp \
		$(SRC_PATH)/Callback/CallbackGetSetAccess.cpp

OBJS 			= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))

THREADPOOL_LINKER				= 	-l$(patsubst lib%.a,%,$(THREADPOOL_LIB))
MEMORYPOOL_LINKER 				= 	-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

LIB_THREADPOOL 					= 	-L./$(THREADPOOL_PATH) $(THREADPOOL_LINKER)
LIB_MEMORYPOOL 					= 	-L./$(MEMORYPOOL_PATH) $(MEMORYPOOL_LINKER)

LIBS 							= 	$(LIB_THREADPOOL) $(LIB_MEMORYPOOL) $(EXTERN_LINKER)

$(TARGET): $(OBJS)

	@$(MAKE) -C $(THREADPOOL_PATH)
	@$(MAKE) -C $(MEMORYPOOL_PATH)

	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(TARGET) vgcore*

fclean: clean

.PHONY: clean

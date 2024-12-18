# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 15:59:23 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/18 09:51:38 by mmaria-d         ###   ########.fr        #
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


SRCS 							=		$(SRC_PATH)/main.cpp															\
																														\
										$(SRC_PATH)/ServerConfig/ServerConfig/ServerConfig.cpp							\
										$(SRC_PATH)/ServerConfig/DefaultConfig/DefaultConfig.cpp						\
										$(SRC_PATH)/ServerConfig/ServerBlock/ServerBlock.cpp							\
										$(SRC_PATH)/ServerConfig/ServerLocation/ServerLocation.cpp						\
																														\
										$(SRC_PATH)/ServerManager/ServerManager/ServerManager.cpp						\
										$(SRC_PATH)/ServerManager/ServerManager/ServerManagerGetSetAccess.cpp			\
										$(SRC_PATH)/ServerManager/ServerManager/ServerManagerPrepareWorkers.cpp			\
										$(SRC_PATH)/ServerManager/ServerWorker/ServerWorker.cpp							\
										$(SRC_PATH)/ServerManager/ServerWorker/ServerWorkerEvents.cpp					\
										$(SRC_PATH)/ServerManager/ServerWorker/ServerWorkerGetSetAccess.cpp				\
										$(SRC_PATH)/ServerManager/ListeningSocket/ListeningSocket.cpp					\
										$(SRC_PATH)/ServerManager/ListeningSocket/ListeningSocketEvents.cpp				\
										$(SRC_PATH)/ServerManager/ListeningSocket/ListeningSocketGetSetAccess.cpp		\
										$(SRC_PATH)/ServerManager/EventManager/EventManager.cpp							\
										$(SRC_PATH)/ServerManager/ConnectionManager/ConnectionManager.cpp				\
										$(SRC_PATH)/ServerManager/ConnectionManager/ManagedConnection.cpp				\
										$(SRC_PATH)/ServerManager/BlockFinder/BlockFinder.cpp							\
																														\
										$(SRC_PATH)/Connection/Connection.cpp											\
										$(SRC_PATH)/Connection/ConnectionGetSetAccess.cpp								\
																														\
										$(SRC_PATH)/Event/Event.cpp														\
										$(SRC_PATH)/Event/EventGetSetAccess.cpp											\
																														\
										$(SRC_PATH)/Globals/Globals.cpp													\
										$(SRC_PATH)/Globals/Clock/Clock.cpp												\
										$(SRC_PATH)/Globals/LogFile/LogFile.cpp											\
										$(SRC_PATH)/Globals/SignalHandler/SignalHandler.cpp								\
																														\
										$(SRC_PATH)/GenericUtils/FileDescriptor/FileDescriptor.cpp						\
										$(SRC_PATH)/GenericUtils/Files/File.cpp											\
										$(SRC_PATH)/GenericUtils/StringUtils/StringUtils.cpp							\
										$(SRC_PATH)/GenericUtils/Validation/Validation.cpp								\
																														\
										$(SRC_PATH)/HttpModule/HttpModule.cpp											\
										$(SRC_PATH)/HttpModule/HttpModuleGetSetAccess.cpp								\
										$(SRC_PATH)/HttpModule/HttpConnection/HttpConnection.cpp						\
										$(SRC_PATH)/HttpModule/HttpManager/HttpManager.cpp								\
										$(SRC_PATH)/HttpModule/HttpRequest/HttpRequest.cpp								\
										$(SRC_PATH)/HttpModule/HttpResponse/HttpResponse.cpp							\
										$(SRC_PATH)/HttpModule/HttpCgiGateway/HttpCgiGateway.cpp						\
																														\
										$(SRC_PATH)/CgiModule/CgiModule/CgiModule.cpp									\
										$(SRC_PATH)/CgiModule/CgiModule/CgiModuleExternal.cpp							\
										$(SRC_PATH)/CgiModule/CgiModule/CgiModuleManagedRequestData.cpp					\
										$(SRC_PATH)/CgiModule/CgiLiveRequest/CgiLiveRequest.cpp							\
										$(SRC_PATH)/CgiModule/CgiLiveRequest/CgiLiveRequestExecute.cpp					\
										$(SRC_PATH)/CgiModule/CgiLiveRequest/CgiLiveRequestEvents.cpp					\
										$(SRC_PATH)/CgiModule/CgiLiveRequest/CgiLiveRequestUtils.cpp					\
										$(SRC_PATH)/CgiModule/CgiRequestData/CgiRequestData.cpp							\
										$(SRC_PATH)/CgiModule/CgiRequestData/CgiRequestDataGetSetAccess.cpp
																			



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



$(NAME): $(OBJS)
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

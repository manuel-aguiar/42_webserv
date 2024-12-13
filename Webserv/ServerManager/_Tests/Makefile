
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out



EXTERN_LINKER 					= 	-lpthread

THREADPOOL_PATH 				= 	../../../Toolkit/ThreadPool
THREADPOOL_LIB					= 	libthreadpool.a

MEMORYPOOL_PATH					= 	../../../Toolkit/MemoryPool
MEMORYPOOL_LIB					= 	libmemorypool.a



SRCS = $(wildcard *.cpp) \
		../ServerManager.cpp \
		../ServerManagerGetSetAccess.cpp \
		../ServerManagerPrepareWorkers.cpp \
											\
		../EventManager/EventManager.cpp \
		../ListeningSocket/ListeningSocket.cpp \
		../ListeningSocket/ListeningSocketGetSetAccess.cpp \
		../ListeningSocket/ListeningSocketEvents.cpp \
		../ConnectionManager/ConnectionManager.cpp \
		../ServerWorker/ServerWorker.cpp \
		../ServerWorker/ServerWorkerGetSetAccess.cpp \
		../ServerWorker/ServerWorkerEvents.cpp \
		../BlockFinder/BlockFinder.cpp \
										\
		../../ServerConfig/_Mock/MockServerConfig.cpp \
		../../ServerConfig/_Mock/MockServerBlock.cpp \
		../../ServerConfig/_Mock/MockServerLocation.cpp \
		../../ServerConfig/_Mock/MockDefaultConfig.cpp \
											\
		../../Globals/_Mock/MockLogFile.cpp \
		../../Globals/_Mock/MockGlobals.cpp \
		../../Globals/_Mock/MockClock.cpp \
											\
		../../Globals/SignalHandler/SignalHandler.cpp \
											\
		../../Connection/Connection.cpp \
		../../Connection/ConnectionGetSetAccess.cpp \
											\
		../../HttpModule/_Mock/MockHttpModule.cpp \
		../../HttpModule/_Mock/MockHttpManager.cpp \
		\
		../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../GenericUtils/StringUtils/StringUtils.cpp \
		../../GenericUtils/Validation/Validation.cpp \
		../../Event/Event.cpp \
		../../Event/EventGetSetAccess.cpp

OBJS = $(SRCS:.cpp=.o)

THREADPOOL_LINKER				= 	-l$(patsubst lib%.a,%,$(THREADPOOL_LIB))
MEMORYPOOL_LINKER 				= 	-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

LIB_THREADPOOL 					= 	-L./$(THREADPOOL_PATH) $(THREADPOOL_LINKER)
LIB_MEMORYPOOL 					= 	-L./$(MEMORYPOOL_PATH) $(MEMORYPOOL_LINKER)

LIBS 							= 	$(LIB_THREADPOOL) $(LIB_MEMORYPOOL) $(EXTERN_LINKER)

$(TARGET): $(OBJS)

	@$(MAKE) -C $(THREADPOOL_PATH)
	@$(MAKE) -C $(MEMORYPOOL_PATH)

	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) 
	@$(MAKE) $(MAKECMDGOALS) -C $(THREADPOOL_PATH)
	@$(MAKE) $(MAKECMDGOALS) -C $(MEMORYPOOL_PATH)

fclean: clean
	rm -rf $(TARGET)
	@$(MAKE) $(MAKECMDGOALS) -C $(THREADPOOL_PATH)
	@$(MAKE) $(MAKECMDGOALS) -C $(MEMORYPOOL_PATH)

.PHONY: clean

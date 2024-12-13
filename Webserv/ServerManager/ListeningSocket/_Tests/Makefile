
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp) \
		../ListeningSocket.cpp \
		../ListeningSocketEvents.cpp \
		../ListeningSocketGetSetAccess.cpp \
											\
		../../_Mock/MockConnectionManager.cpp \
		../../_Mock/MockServerManager.cpp \
		../../_Mock/MockEventManager.cpp \
		../../_Mock/MockServerWorker.cpp \
		../../_Mock/MockBlockFinder.cpp \
										\
		../../../ServerConfig/_Mock/MockServerConfig.cpp \
		../../../ServerConfig/_Mock/MockServerBlock.cpp \
		../../../ServerConfig/_Mock/MockServerLocation.cpp \
		../../../ServerConfig/_Mock/MockDefaultConfig.cpp \
											\
		../../../Globals/_Mock/MockLogFile.cpp \
		../../../Globals/_Mock/MockGlobals.cpp \
		../../../Globals/_Mock/MockClock.cpp \
											\
		../../../Connection/_Mock/MockConnection.cpp \
		../../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../../GenericUtils/StringUtils/StringUtils.cpp \
		../../../GenericUtils/Validation/Validation.cpp \
		../../../Event/Event.cpp \
		../../../Event/EventGetSetAccess.cpp \
											\
		../../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp \
		../../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp 

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --leak-check=full --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(TARGET)

.PHONY: clean

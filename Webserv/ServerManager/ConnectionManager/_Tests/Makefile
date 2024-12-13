
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp) ../ConnectionManager.cpp \
		../../../Globals/_Mock/MockLogFile.cpp \
		../../../Globals/_Mock/MockGlobals.cpp \
		../../../Globals/_Mock/MockClock.cpp \
		../../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../../Connection/Connection.cpp \
		../../../Event/Event.cpp \
		../../../Connection/ConnectionGetSetAccess.cpp \
		../../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp \
		../../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp 

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(TARGET)

.PHONY: clean

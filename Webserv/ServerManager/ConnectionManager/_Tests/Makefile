
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test

SRCS = $(wildcard *.cpp) ../ConnectionManager.cpp \
		../../../Globals/LogFile/LogFile.cpp \
		../../../Globals/Globals.cpp \
		../../../Globals/Clock/Clock.cpp \
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

clean:
	rm -rf $(OBJS) $(TARGET) debug.log error.log status.log

fclean:
	rm -rf $(OBJS) $(TARGET) debug.log error.log status.log

.PHONY: clean

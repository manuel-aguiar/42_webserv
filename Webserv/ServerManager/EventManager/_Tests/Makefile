
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp) ../EventManager.cpp \
		../../../Globals/LogFile/LogFile.cpp \
		../../../Globals/Globals.cpp \
		../../../Globals/Clock/Clock.cpp \
		../../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../../Event/Event.cpp \
		../../../Event/EventGetSetAccess.cpp \
		../../../Connection/ConnectionGetSetAccess.cpp

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
	rm -rf $(TARGET) debug.log error.log status.log

.PHONY: clean

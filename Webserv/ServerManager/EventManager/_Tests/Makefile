
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp) ../EventManager.cpp \
		../../../Globals/_Mock/MockLogFile.cpp \
		../../../Globals/_Mock/MockGlobals.cpp \
		../../../Globals/_Mock/MockClock.cpp \
		../../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../../Event/Event.cpp \
		../../../Event/EventGetSetAccess.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)
	make fclean

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(TARGET)

.PHONY: clean

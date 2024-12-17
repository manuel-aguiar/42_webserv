
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

fclean: clean
	$(TARGET)

re: fclean all

.PHONY: clean fclean run

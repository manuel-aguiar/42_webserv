#
# This is just a Makefile to compile tests
# It must be adjusted according to dependencies of tests themselves
#

NAME		= test.out
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g -DTESTMODE

# Directories
TEST_DIR = .
SRC_DIR = ..
PARSER_DIR = ../parsers
HTTP_MODULE_DIR = ../..

# Source files
SRCS		= test.cpp \
			  ../BlockFinder.cpp

# Object files
OBJS		= $(SRCS:.cpp=.o)

# Include paths
INCLUDES = -I$(SRC_DIR) \
           -I../../../GenericUtils \
           -I.

# Rules
all: clean_objs $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Pattern rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean_objs:
	rm -f $(OBJS)

clean: clean_objs
	rm -f $(NAME)

fclean: clean

re: fclean all

run: all
	@echo "\nRunning tests with valgrind...\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

test: all
	./$(NAME)

.PHONY: all clean clean_objs fclean re run test

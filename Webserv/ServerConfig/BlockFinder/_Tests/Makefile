#
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

NAME		= blockfinder_test
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g

# Directories
TEST_DIR = .
SRC_DIR = ..
PARSER_DIR = ../parsers
HTTP_MODULE_DIR = ../..

# Target
NAME = test.out

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
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Pattern rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	@echo "\nRunning tests with valgrind...\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

test: all
	./$(NAME)

.PHONY: all clean fclean re run test

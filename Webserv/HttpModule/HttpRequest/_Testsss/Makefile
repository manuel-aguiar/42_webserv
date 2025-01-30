#
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# Directories
TEST_DIR = .
SRC_DIR = ..
PARSER_DIR = ../parsers
HTTP_MODULE_DIR = ../..

# Target
NAME = test.out

# Source files
TEST_SRCS = $(TEST_DIR)/test_req_line.cpp

MAIN_SRCS = $(SRC_DIR)/HttpRequest.cpp \
            $(HTTP_MODULE_DIR)/HttpDefinitions.cpp

PARSER_SRCS = $(PARSER_DIR)/http_parse_request_line.cpp \
              $(PARSER_DIR)/http_parse_header_line.cpp \
              $(PARSER_DIR)/http_parse_message_body.cpp

# Object files
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
PARSER_OBJS = $(PARSER_SRCS:.cpp=.o)

# All objects
OBJS = $(TEST_OBJS) $(MAIN_OBJS) $(PARSER_OBJS)

# Include paths
INCLUDES = -I$(SRC_DIR)

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

.PHONY: all clean fclean re run

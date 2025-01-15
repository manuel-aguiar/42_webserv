#
# This is just a Makefile to compile tests
# It must be adjusted according to dependencies of tests themselves
#

# Compiler and flags
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g -DTESTMODE

# Directories
ROOT_DIR	= ../../..
UTILS_DIR	= $(ROOT_DIR)/GenericUtils
CONFIG_DIR	= $(ROOT_DIR)/ServerConfig
TEST_DIR	= .

# Target
NAME		= test.out

# Source directories
SRCS_DIRS	= $(TEST_DIR) \
			  $(CONFIG_DIR)/BlockFinder \
			  $(CONFIG_DIR)/ServerBlock \
			  $(CONFIG_DIR)/ServerLocation \
			  $(CONFIG_DIR)/DefaultConfig \
			  $(UTILS_DIR)/StringUtils \
			  $(UTILS_DIR)/Validation

# Source files
SRCS		= $(TEST_DIR)/test.cpp \
			  $(CONFIG_DIR)/BlockFinder/BlockFinder.cpp \
			  $(CONFIG_DIR)/ServerBlock/ServerBlock.cpp \
			  $(CONFIG_DIR)/ServerLocation/ServerLocation.cpp \
			  $(CONFIG_DIR)/DefaultConfig/DefaultConfig.cpp \
			  $(UTILS_DIR)/StringUtils/StringUtils.cpp \
			  $(UTILS_DIR)/Validation/Validation.cpp

# Object files (maintain directory structure)
OBJS		= $(SRCS:%.cpp=%.o)

# Include paths
INCLUDES	= -I$(CONFIG_DIR)/BlockFinder \
			  -I$(CONFIG_DIR)/ServerBlock \
			  -I$(CONFIG_DIR)/DefaultConfig \
			  -I$(CONFIG_DIR)/ServerLocation \
			  -I$(UTILS_DIR) \
			  -I$(UTILS_DIR)/StringUtils \
			  -I$(UTILS_DIR)/Validation \
			  -I$(TEST_DIR)

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Pattern rule for object files
%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	find $(ROOT_DIR) -type d -empty -delete 2>/dev/null || true

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	@echo "\nRunning integration tests with valgrind...\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

test: all
	./$(NAME)

.PHONY: all clean fclean re run test

# Dependencies
-include $(OBJS:.o=.d)

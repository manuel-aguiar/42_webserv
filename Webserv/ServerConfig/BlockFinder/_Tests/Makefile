#
# This is just a Makefile to compile tests
# It must be adjusted according to dependencies of tests themselves
#

TARGET_DEBUG		=	debug.out
TARGET_TEST			=	test.out
CXX					= 	c++
CXXFLAGS			= 	-Wall -Wextra -Werror -std=c++98 -g -MMD -MP -DTESTMODE

# Directories
ROOT_PATH 			= 	$(shell git rev-parse --show-toplevel)
SRC_PATH			=	$(ROOT_PATH)/.
OBJ_PATH			=	$(ROOT_PATH)/_build

# main target, in this case blockfinder
SUBJECT_PATH 		=	$(SRC_PATH)/Webserv/ServerConfig/BlockFinder

TEST_FOLDERNAME		=	_Tests

# Source files
SRCS_DEBUG			= 	$(SUBJECT_PATH)/$(TEST_FOLDERNAME)/debug.cpp 
SRCS_TEST			= 	$(SUBJECT_PATH)/$(TEST_FOLDERNAME)/main.cpp

SRCS_COMMON			=	$(SUBJECT_PATH)/BlockFinder.cpp \
						\
						$(SUBJECT_PATH)/$(TEST_FOLDERNAME)/common.cpp \
						$(SUBJECT_PATH)/$(TEST_FOLDERNAME)/MockServerBlock.cpp \
						$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
						$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp \
						$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
						$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp
						

# Object files
OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_DEBUG				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_DEBUG))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

# dependencies
DEPS_COMMON 			= 		$(OBJS_COMMON:.o=.d)
DEPS_DEBUG 				= 		$(OBJS_DEBUG:.o=.d)
DEPS_TEST 				= 		$(OBJS_TEST:.o=.d)

# Rules
all: clean_objs $(NAME)


$(TARGET_DEBUG): $(OBJS_COMMON) $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_DEBUG) -o $(TARGET_DEBUG)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_DEBUG) $(DEPS_TEST)

clean_objs:
	rm -f $(OBJS)

clean: clean_objs
	rm -f $(NAME)

fclean: clean

re: fclean all

run: all
	@echo "\nRunning tests with valgrind...\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

debug: $(TARGET_DEBUG)
	@valgrind --quiet --leak-check=full --show-leak-kinds=all ./$(TARGET_DEBUG)

test: $(TARGET_TEST)
	@valgrind --quiet --leak-check=full --show-leak-kinds=all ./$(TARGET_TEST)


.PHONY: all clean clean_objs fclean re run test

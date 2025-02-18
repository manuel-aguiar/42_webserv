
# Makefile

TARGET_TEST 			=		test.out
CXX						=		c++
CXXFLAGS 				=		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build/debug

SRCS_TEST				=		$(SRC_PATH)/Webserv/ServerConfig/_Tests/Internals/_main.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/_Tests/Internals/_test1.cpp

SRCS_COMMON				=		$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp  \
								$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp 


OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

DEPS_COMMON 			= 		$(OBJS_COMMON:.o=.d)
DEPS_TEST 				= 		$(OBJS_TEST:.o=.d)

#=== COLORS ===#

RED =		\033[0;31m
GREEN =		\033[0;92m
YELLOW =	\033[93m
BLUE =		\033[0;34m
MAGENTA =	\033[0;35m
CYAN =		\033[96m
RESET =		\033[0m

TAG =		$(CYAN)[$(NAME)]$(RESET)

#=== RULES ===#

all: $(TARGET_TEST)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_TEST)

clean:
	@echo "$(TAG) Cleaning object files."
	@rm -rf $(OBJ_PATH)

fclean: clean
	@echo "$(TAG) Full clean."
	@rm -f $(NAME)

test: $(TARGET_TEST)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_TEST)

debug: test

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re

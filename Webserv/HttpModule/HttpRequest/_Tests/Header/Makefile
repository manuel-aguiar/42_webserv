# Makefile

# TARGET_DEBUG 			=		debug_Request.out
TARGET_TEST 			=		test_Request.out
CXX						=		c++
CXXFLAGS 				=		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

SRCS_TEST				=		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/_Tests/Header/main.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/_Tests/Header/headerTests.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/_Tests/Header/bodyTests.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/_Tests/Header/lineTests.cpp



# SRCS_DEBUG				=		$(SRC_PATH)/Webserv/ServerConfig/_Tests/ServerConfigTests.cpp \

SRCS_COMMON				=		$(SRC_PATH)/Webserv/HttpModule/HttpDefinitions.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_header_line.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_message_body.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_request_line.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp


OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_DEBUG				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_DEBUG))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

DEPS_COMMON 			= 		$(OBJS_COMMON:.o=.d)
DEPS_DEBUG 				= 		$(OBJS_DEBUG:.o=.d)
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

all: $(TARGET_TEST) # $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJS_COMMON) $(OBJS_DEBUG)
	@echo "$(CYAN)[$(TARGET_DEBUG)]$(RESET) Making Program"
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_DEBUG) -o $(TARGET_DEBUG)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_DEBUG) $(DEPS_TEST)

clean:
	@echo "$(TAG) Cleaning object files."
	@rm -rf $(OBJ_PATH)

fclean: clean
	@echo "$(TAG) Full clean."
	@rm -f $(TARGET_TEST)

test: $(TARGET_TEST)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_TEST)

debug: $(TARGET_DEBUG)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_DEBUG)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re

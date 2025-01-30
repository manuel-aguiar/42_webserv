


NAME =			TestServerConfig.out
CXX =			c++
CXXFLAGS =		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

SRCS					=		$(SRC_PATH)/Webserv/ServerConfig/_Tests/main.cpp \
								\
								$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp  \
								$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp 


OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

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

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(TAG) Making Program"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)


$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:

fclean: clean
	@echo "$(TAG) Full clean."
	@rm -f $(NAME)

test: $(NAME)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(NAME)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re

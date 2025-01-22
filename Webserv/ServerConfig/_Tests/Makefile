.SILENT:
NAME =			TestServerConfig.out
CXX =			c++
CXXFLAGS =		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP #-fsanitize=address
SRC =			ServerConfigTests.cpp ../ServerConfig/ServerConfig.cpp ../ServerConfig/ServerConfigGetAddrInfo.cpp  ../ServerBlock/ServerBlock.cpp \
				../DefaultConfig/DefaultConfig.cpp ../ServerLocation/ServerLocation.cpp \
				../../GenericUtils/StringUtils/StringUtils.cpp \
				../../GenericUtils/Validation/Validation.cpp 
DEPS =			$(SRC:.cpp=.d)

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

$(NAME): $(SRC)
	@echo "$(TAG) Making Program"
	@$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

-include $(DEPS)

clean:

fclean: clean
	@echo "$(TAG) Full clean."
	@rm -f $(NAME)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re

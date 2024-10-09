.SILENT:
NAME =			a.out
CXX =			c++
CXXFLAGS =		-Wall -Wextra -Werror -g #-std=c++98
SRC =			./src/main.cpp ./src/ConfigHandler.cpp ./src/ServerConfig.cpp ./src/utils.cpp
INCLUDE_DIR =	./inc/


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
	@$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME) -I $(INCLUDE_DIR)

clean:

fclean: clean
	@echo "$(TAG) Full clean." 
	@rm -f $(NAME)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re
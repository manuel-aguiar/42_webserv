


NAME =			test.out
CXX =			c++
CXXFLAGS =		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP #-fsanitize=address

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build/debug

SRCS					=		$(SRC_PATH)/Webserv/CgiModule/_Tests/Parsing/main.cpp \
								\
								$(SRC_PATH)/Webserv/GenericUtils/BufferView/BufferView.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Buffer/Buffer.cpp

								
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


#=== RULES ===#

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)


$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf *vgcore*

fclean: clean
	@rm -f $(NAME)

test: $(NAME)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(NAME)

re: fclean all
	@echo "$(TAG) Recompiling."


debug: test


.PHONY: all clean fclean re

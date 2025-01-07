# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2025/01/07 11:46:33 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME = test.out

# Paths
ROOT_PATH = ../../..
SRC_PATH = $(ROOT_PATH)/.
OBJ_PATH = $(ROOT_PATH)/Webserv/_build

# Sources
SRCS    =       $(SRC_PATH)/Webserv/Event/Event.cpp \
				$(SRC_PATH)/Webserv/Event/EventGetSetAccess.cpp \
				\
				$(SRC_PATH)/Webserv/Event/_Tests/main.cpp \
                \
                $(SRC_PATH)/Webserv/Callback/Callback.cpp \
                $(SRC_PATH)/Webserv/Callback/CallbackGetSetAccess.cpp \
                \
                $(SRC_PATH)/Toolkit/_Tests/testHelpers.cpp


OBJS 			= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS 			= 	$(OBJS:.o=.d)

#####################################################

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

#####################################################

all: $(NAME)

test: $(NAME)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(NAME)

clean:
	rm -rf $(NAME) vgcore*

fclean: clean

re: fclean all

.PHONY: clean fclean run

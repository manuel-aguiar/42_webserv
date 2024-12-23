# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/23 13:47:46 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME					=		test.out

# Paths
ROOT_PATH				=		../../../../Toolkit
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

MEMORYPOOL_PATH			=	 	$(ROOT_PATH)/MemoryPool
MEMORYPOOL_LIB			=	 	libmemorypool.a

MEMORYPOOL_LINKER 		=		-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

LIB_MEMORYPOOL 			=		-L$(MEMORYPOOL_PATH) $(MEMORYPOOL_LINKER)

# Sources
SRCS					=		$(SRC_PATH)/Arrays/DynArray/_Tests/test.cpp	\
								\
								$(SRC_PATH)/_Tests/ToolkitDummy.cpp			\
								$(SRC_PATH)/_Tests/ToolkitBase.cpp			\
								$(SRC_PATH)/_Tests/ToolkitDerived.cpp

OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

#####################################################

$(NAME): $(OBJS)
	@$(MAKE) -C $(MEMORYPOOL_PATH) --quiet
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB_MEMORYPOOL) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

#####################################################

all: $(NAME)

run: $(NAME)
	valgrind  --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(NAME)

clean:
	rm -rf $(NAME) vgcore*

fclean: clean

re: fclean all

.PHONY: clean fclean run

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/27 15:12:02 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME					=		test.out

# Paths
ROOT_PATH				=		../../../Toolkit/ThreadPool
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

EXTERN_LINKER 					= 	-lpthread

DEBUG_FLAGS 					= 	-g
BUILD_FLAGS 					= 	-DNDEBUG

THREADPOOL_PATH 				= 	$(ROOT_PATH)
THREADPOOL_LIB					= 	libthreadpool.a

MEMORYPOOL_LINKER 			=		-l$(patsubst lib%.a,%,$(MEMORYPOOL_LIB))

THREADPOOL_LINKER				= 	-l$(patsubst lib%.a,%,$(THREADPOOL_LIB))

LIB_THREADPOOL 					= 	-L./$(THREADPOOL_PATH) $(THREADPOOL_LINKER)

LIBS 							= 	$(LIB_THREADPOOL) $(EXTERN_LINKER)

# Sources
SRCS					=		$(SRC_PATH)/_Tests/test.cpp					\
								$(SRC_PATH)/_Tests/testPromiseFuture.cpp

OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

#####################################################

$(NAME): $(OBJS) $(ROOT_PATH)/$(THREADPOOL_LIB)
	@$(MAKE) -C $(THREADPOOL_PATH) --quiet
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ROOT_PATH)/$(THREADPOOL_LIB):
	@$(MAKE) -C $(THREADPOOL_PATH) --quiet

-include $(DEPS)

#####################################################

all: $(NAME)

test: $(NAME)
	@valgrind --quiet --tool=helgrind ./$(NAME)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(NAME)

clean:
	rm -rf $(NAME) vgcore*

fclean: clean

re: fclean
	@$(MAKE) -C $(THREADPOOL_PATH) re --quiet
	make

.PHONY: clean fclean run

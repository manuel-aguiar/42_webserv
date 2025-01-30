

# Compiler and flags
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME					=		test.out

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)
OBJ_PATH				=		$(ROOT_PATH)/_build

# Sources
SRCS					=		$(SRC_PATH)/Webserv/CgiModule/TimerTracker/_Tests/main.cpp		\
								$(SRC_PATH)/Webserv/CgiModule/TimerTracker/_Tests/test1.cpp		\
								\
								$(SRC_PATH)/Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp	\
								$(SRC_PATH)/Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp

OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

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

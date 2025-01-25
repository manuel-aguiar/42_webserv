

# Compiler and flags
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME					=		test.out

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/Toolkit
OBJ_PATH				=		$(ROOT_PATH)/_build

EXTERN_LINKER 			= 		-lpthread

DEBUG_FLAGS 			= 		-g
BUILD_FLAGS 			= 		-DNDEBUG


LIBS 					= 		$(EXTERN_LINKER)

# Sources
SRCS					=		$(SRC_PATH)/ThreadPool/_Tests/main.cpp					\
								$(SRC_PATH)/ThreadPool/_Tests/test1.cpp					\
								$(SRC_PATH)/ThreadPool/_Tests/test2.cpp					\
								$(SRC_PATH)/ThreadPool/_Tests/test3.cpp					\
								$(SRC_PATH)/ThreadPool/_Tests/test4.cpp				

OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

#####################################################

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


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
	make

.PHONY: clean fclean run

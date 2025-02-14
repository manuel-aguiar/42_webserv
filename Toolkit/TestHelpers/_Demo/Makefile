

# Compiler and flags
TARGET_TEST				=		test.out
TARGET_DEBUG			=		debug.out
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)
OBJ_PATH				=		$(ROOT_PATH)/_build/debug

SUBJECT_PATH			=		$(SRC_PATH)/Toolkit/TestHelpers/_Demo
# Sources
SRCS_COMMON				=		$(SUBJECT_PATH)/common.cpp
SRCS_TEST				=		$(SUBJECT_PATH)/test.cpp
SRCS_DEBUG				=		$(SUBJECT_PATH)/debug.cpp

OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))
OBJS_DEBUG				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_DEBUG))

DEPS_COMMON				=		$(OBJS_COMMON:.o=.d)
DEPS_TEST				=		$(OBJS_TEST:.o=.d)
DEPS_DEBUG				=		$(OBJS_DEBUG:.o=.d)

#####################################################

all: $(TARGET_TEST) $(TARGET_DEBUG)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(TARGET_DEBUG): $(OBJS_COMMON) $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_DEBUG) -o $(TARGET_DEBUG)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_TEST) $(DEPS_DEBUG)

#####################################################

all: $(NAME)

test: $(TARGET_TEST)
	@valgrind  --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(TARGET_TEST)

debug: $(TARGET_DEBUG)
	@valgrind  --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(TARGET_DEBUG)

clean:
	rm -rf vgcore*

fclean: clean
	rm -rf $(TARGET_TEST) $(TARGET_DEBUG) $(OBJ_PATH)

re: fclean all

.PHONY: clean fclean run

# Compiler and flags
TARGET_TEST             =       test.out
CXX                     =       c++
CXXFLAGS                =       -Wall -Wextra -Werror -g -std=c++98 -MMD -MP

# Paths
ROOT_PATH               =       $(shell git rev-parse --show-toplevel)
SRC_PATH                =       $(ROOT_PATH)
OBJ_PATH                =       $(ROOT_PATH)/_build

SUBJECT_PATH            =       $(SRC_PATH)/Webserv

# Sources
SRCS_TEST               =       $(SUBJECT_PATH)/Globals/LogFile/_Tests/test.cpp  \
								\
								$(SUBJECT_PATH)/Globals/LogFile/LogFile.cpp \
								$(SUBJECT_PATH)/Globals/Globals.cpp \
								$(SUBJECT_PATH)/Globals/Clock/Clock.cpp

OBJS_TEST               =       $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

DEPS_TEST               =       $(OBJS_TEST:.o=.d)

#####################################################

all: $(TARGET_TEST)

$(TARGET_TEST): $(OBJS_TEST)
	$(CXX) $(CXXFLAGS)  $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_TEST)

#####################################################

test: $(TARGET_TEST)
	@valgrind  --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_TEST)

debug: test

clean:
	rm -rf vgcore*

fclean: clean
	rm -rf $(TARGET_TEST) $(OBJ_PATH)

re: fclean all

.PHONY: clean fclean run test debug

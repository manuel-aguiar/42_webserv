


# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MMD -MP
NAME = test.out

# Paths
ROOT_PATH	=	$(shell git rev-parse --show-toplevel)
SRC_PATH	=	$(ROOT_PATH)/.
OBJ_PATH	=	$(ROOT_PATH)/_build


SRCS		 =  $(SRC_PATH)/Webserv/EventManager/EventManager/EventManager.cpp \
				$(SRC_PATH)/Webserv/EventManager/InternalEvent/InternalEvent.cpp \
				$(SRC_PATH)/Webserv/EventManager/_Tests/main.cpp \
				$(SRC_PATH)/Webserv/EventManager/_Tests/test1.cpp \
				$(SRC_PATH)/Webserv/EventManager/_Tests/test2.cpp \
													\
				$(SRC_PATH)/Webserv/Globals/_Mock/MockLogFile.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockGlobals.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockClock.cpp \
													\
				$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp \
				$(SRC_PATH)/Webserv/EventCallback/EventCallback.cpp \
				$(SRC_PATH)/Webserv/EventCallback/EventCallbackGetSetAccess.cpp \
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
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(NAME)

clean:
	rm -rf $(NAME) vgcore*

fclean: clean

re: fclean all

.PHONY: clean fclean run

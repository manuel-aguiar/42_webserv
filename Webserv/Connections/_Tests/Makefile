


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

SRCS		 =  $(SRC_PATH)/Webserv/Connections/_Tests/main.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testSocket.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testMonitor.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testListeningSocket.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testAppLayer.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testAccepter.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testConnection.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/testManager.cpp \
				$(SRC_PATH)/Webserv/Connections/_Tests/common.cpp \
				\
				$(SRC_PATH)/Webserv/Connections/Manager/Manager.cpp \
				$(SRC_PATH)/Webserv/Connections/Connection/Connection.cpp \
				$(SRC_PATH)/Webserv/Connections/Connection/ConnectionGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManager.cpp \
				$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManagerInitShutdown.cpp \
				$(SRC_PATH)/Webserv/Connections/InternalConn/InternalConn.cpp \
				$(SRC_PATH)/Webserv/Connections/ListeningSocket/ListeningSocket.cpp \
				$(SRC_PATH)/Webserv/Connections/ListeningSocket/ListeningSocketOpenClose.cpp \
				$(SRC_PATH)/Webserv/Connections/ListeningSocket/ListeningSocketAccept.cpp \
				$(SRC_PATH)/Webserv/Connections/Monitor/Monitor.cpp \
				$(SRC_PATH)/Webserv/Connections/AppLayer/AppLayer.cpp \
				$(SRC_PATH)/Webserv/Connections/Socket/Socket.cpp \
				$(SRC_PATH)/Webserv/Connections/Accepter/Accepter.cpp \
				\
				$(SRC_PATH)/Webserv/Events/Manager/Manager.cpp \
				$(SRC_PATH)/Webserv/Events/Subscription/Subscription.cpp \
				$(SRC_PATH)/Webserv/Events/Subscription/SubscriptionGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/Events/InternalSub/InternalSub.cpp \
				\
				$(SRC_PATH)/Webserv/ServerContext/ServerContext.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockLogFile.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockGlobals.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockClock.cpp \
													\
				$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp

OBJS 			= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS 			= 	$(OBJS:.o=.d)

#####################################################

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) -lpthread

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

#####################################################

all: $(NAME)

test: $(NAME)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all --trace-children=yes ./$(NAME)

clean:
	rm -rf $(NAME) vgcore*

fclean: clean

re: fclean all

.PHONY: clean fclean run

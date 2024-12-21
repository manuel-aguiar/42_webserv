# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2024/12/21 12:47:54 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MMD -MP
TARGET = test.out

# Paths
ROOT_PATH = ../../../Webserv
SRC_PATH = $(ROOT_PATH)/.
OBJ_PATH = $(ROOT_PATH)/CgiModule/_Tests/_build

# Sources
SRCS =  $(SRC_PATH)/CgiModule/_Tests/test.cpp \
        $(SRC_PATH)/CgiModule/_Tests/TestProtoConnections/A_ProtoRequest.cpp \
        $(SRC_PATH)/CgiModule/_Tests/TestProtoConnections/A_ProtoRequestCgiGateway.cpp \
        $(SRC_PATH)/CgiModule/_Tests/TestProtoConnections/A_ProtoRequestReadWrite.cpp \
						\
						\
        $(SRC_PATH)/CgiModule/CgiModule/CgiModule.cpp \
        $(SRC_PATH)/CgiModule/CgiModule/CgiModuleExternal.cpp \
        $(SRC_PATH)/CgiModule/CgiModule/CgiModuleInternal.cpp \
        $(SRC_PATH)/CgiModule/CgiModule/CgiModuleGetSetAccess.cpp \
        $(SRC_PATH)/CgiModule/InternalCgiRequestData/InternalCgiRequestData.cpp \
        $(SRC_PATH)/CgiModule/InternalCgiWorker/InternalCgiWorker.cpp \
        $(SRC_PATH)/CgiModule/InternalCgiWorker/InternalCgiWorkerExecute.cpp \
        $(SRC_PATH)/CgiModule/CgiRequestData/CgiRequestData.cpp \
        $(SRC_PATH)/CgiModule/CgiRequestData/CgiRequestDataGetSetAccess.cpp \
        $(SRC_PATH)/ServerManager/EventManager/EventManager.cpp \
        $(SRC_PATH)/Globals/_Mock/MockLogFile.cpp \
        $(SRC_PATH)/Globals/_Mock/MockGlobals.cpp \
        $(SRC_PATH)/Globals/_Mock/MockClock.cpp \
        $(SRC_PATH)/Globals/SignalHandler/SignalHandler.cpp \
        $(SRC_PATH)/HttpModule/_Mock/MockHttpModule.cpp \
        $(SRC_PATH)/HttpModule/_Mock/MockHttpManager.cpp \
        $(SRC_PATH)/GenericUtils/FileDescriptor/FileDescriptor.cpp \
        $(SRC_PATH)/GenericUtils/StringUtils/StringUtils.cpp \
        $(SRC_PATH)/GenericUtils/Validation/Validation.cpp \
        $(SRC_PATH)/Event/Event.cpp \
        $(SRC_PATH)/Event/EventGetSetAccess.cpp \
        $(SRC_PATH)/Callback/Callback.cpp \
        $(SRC_PATH)/Callback/CallbackGetSetAccess.cpp


# Derived object and dependency files
OBJS 			= 	$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS 			= 	$(OBJS:.o=.d)

# Target rules
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile sources to objects, creating necessary subdirectories
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependencies
-include $(DEPS)

# Standard targets
all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJ_PATH) vgcore*

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: clean fclean run

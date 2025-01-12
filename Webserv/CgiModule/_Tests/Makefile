# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2025/01/12 00:09:41 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MMD -MP
TARGET_BASE = functional_test.out
TARGET_STRESSTEST = stress_test.out
TARGET_STRESSVALG = val_stress.out

VALGRIND_OUTPUT = valgrind_output.log

# Paths
ROOT_PATH	=	$(shell git rev-parse --show-toplevel)
SRC_PATH	=	$(ROOT_PATH)/.
OBJ_PATH	=	$(ROOT_PATH)/_build

# Sources

FUNC_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/main_FunctionalTest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/test1.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/test2.cpp

STRESS_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/main_StressTest.cpp

STRESSVALG_SRCS = $(SRC_PATH)/Webserv/CgiModule/_Tests/main_StressValgrind.cpp

COMMON_SRCS =   $(SRC_PATH)/Webserv/CgiModule/_Tests/CgiStressTest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/CgiStressTest_ExpectedOutput.cpp \
				\
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoConnections/A_ProtoRequest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoConnections/A_ProtoRequestCgiGateway.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoConnections/A_ProtoRequest_onCallbacksCgi.cpp \
									\
				$(SRC_PATH)/Toolkit/_Tests/testHelpers.cpp \
									\
				$(SRC_PATH)/Webserv/CgiModule/CgiModule/CgiModule.cpp \
				$(SRC_PATH)/Webserv/CgiModule/CgiModule/CgiModuleExternal.cpp \
				$(SRC_PATH)/Webserv/CgiModule/CgiModule/CgiModuleInternal.cpp \
				$(SRC_PATH)/Webserv/CgiModule/CgiModule/CgiModuleGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalCgiRequestData/InternalCgiRequestData.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalCgiWorker/InternalCgiWorker.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalCgiWorker/InternalCgiWorkerEvents.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalCgiWorker/InternalCgiWorkerExecute.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalCgiWorker/InternalCgiWorkerHelpers.cpp \
				$(SRC_PATH)/Webserv/CgiModule/CgiRequestData/CgiRequestData.cpp \
				$(SRC_PATH)/Webserv/CgiModule/CgiRequestData/CgiRequestDataGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Timer/Timer.cpp \
				$(SRC_PATH)/Webserv/ServerManager/EventManager/EventManager.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockLogFile.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockGlobals.cpp \
				$(SRC_PATH)/Webserv/Globals/_Mock/MockClock.cpp \
				$(SRC_PATH)/Webserv/Globals/SignalHandler/SignalHandler.cpp \
				$(SRC_PATH)/Webserv/HttpModule/_Mock/MockHttpModule.cpp \
				$(SRC_PATH)/Webserv/HttpModule/_Mock/MockHttpManager.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp \
				$(SRC_PATH)/Webserv/Event/Event.cpp \
				$(SRC_PATH)/Webserv/Event/EventGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/Callback/Callback.cpp \
				$(SRC_PATH)/Webserv/Callback/CallbackGetSetAccess.cpp \


OBJS_COMMON = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(COMMON_SRCS))
OBJS_BASE = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(FUNC_SRCS))
OBJS_STRESS = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(STRESS_SRCS))
OBJS_STRESSVALG = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(STRESSVALG_SRCS))

DEPS_COMMON = $(OBJS_COMMON:.o=.d)
DEPS_BASE = $(OBJS_BASE:.o=.d)
DEPS_STRESS = $(OBJS_STRESS:.o=.d)
DEPS_STRESSVALG = $(OBJS_STRESSVALG:.o=.d)

$(TARGET_BASE): $(OBJS_COMMON) $(OBJS_BASE)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_STRESSTEST): $(OBJS_COMMON) $(OBJS_STRESS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_STRESSVALG): $(OBJS_COMMON) $(OBJS_STRESSVALG)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile sources to objects, creating necessary subdirectories
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependencies
-include $(DEPS_COMMON) $(DEPS_BASE) $(DEPS_STRESS) $(DEPS_STRESSVALG)

##chatgpt magic to generate suppresion files
#valgrind --gen-suppressions=all --leak-check=full --show-leak-kinds=all python3 TestScripts/py/envPrint.py  2>&1 | awk '/^{/{flag=1} flag; /}$/ {flag=0}' > python.supp && cat python.supp

# Standard targets
all: $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG)

test: $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_BASE)
	@valgrind   \
	--log-file=$(VALGRIND_OUTPUT) \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_STRESSVALG)
	@./$(TARGET_STRESSTEST)
##        --track-fds=yes 
##        --track-origins=yes

clean:
	rm -rf $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG) vgcore* $(VALGRIND_OUTPUT)

fclean: clean


re: fclean all

.PHONY: clean fclean run

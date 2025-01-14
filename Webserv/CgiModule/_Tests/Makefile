# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/21 12:36:48 by mmaria-d          #+#    #+#              #
#    Updated: 2025/01/14 15:32:37 by mmaria-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MMD -MP
TARGET_BASE = functional_test.out
TARGET_STRESSTEST = stress_test.out
TARGET_STRESSVALG = val_stress.out
TARGET_STRESSCHILDREN = children_stress.out

VALGRIND_OUTPUT = valgrind_output.log

# Paths
ROOT_PATH	=	$(shell git rev-parse --show-toplevel)
SRC_PATH	=	$(ROOT_PATH)/.
OBJ_PATH	=	$(ROOT_PATH)/_build

# Sources

FUNC_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/main_FunctionalTest.cpp

STRESS_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/main_StressTest.cpp

STRESSVALG_SRCS = $(SRC_PATH)/Webserv/CgiModule/_Tests/main_StressValgrind.cpp

STRESSCHILDREN = $(SRC_PATH)/Webserv/CgiModule/_Tests/main_StressChildren.cpp

COMMON_SRCS =   $(SRC_PATH)/Webserv/CgiModule/_Tests/CgiStressTest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/CgiStressTest_ExpectedOutput.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/test1.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/test2.cpp \
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
				\
				$(SRC_PATH)/Webserv/TimerTracker/Timer/Timer.cpp \
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

#################################################################
OBJS_COMMON = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(COMMON_SRCS))
OBJS_BASE = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(FUNC_SRCS))
OBJS_STRESS = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(STRESS_SRCS))
OBJS_STRESSVALG = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(STRESSVALG_SRCS))
OBJS_STRESSCHILDREN = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(STRESSCHILDREN))


##########################################################
DEPS_COMMON = $(OBJS_COMMON:.o=.d)
DEPS_BASE = $(OBJS_BASE:.o=.d)
DEPS_STRESS = $(OBJS_STRESS:.o=.d)
DEPS_STRESSVALG = $(OBJS_STRESSVALG:.o=.d)
DEPS_STRESSCHILDREN = $(OBJS_STRESSCHILDREN:.o=.d)

###############################################################3

all: $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG) $(TARGET_STRESSCHILDREN)

###################################################################

$(TARGET_BASE): $(OBJS_COMMON) $(OBJS_BASE)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_STRESSTEST): $(OBJS_COMMON) $(OBJS_STRESS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_STRESSVALG): $(OBJS_COMMON) $(OBJS_STRESSVALG)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_STRESSCHILDREN): $(OBJS_COMMON) $(OBJS_STRESSCHILDREN)
	$(CXX) $(CXXFLAGS) -o $@ $^

#################################################################
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

################################################################################################
-include $(DEPS_COMMON) $(DEPS_BASE) $(DEPS_STRESS) $(DEPS_STRESSVALG) $(DEPS_STRESSCHILDREN)

##chatgpt magic to generate suppresion files
#valgrind --gen-suppressions=all --leak-check=full --show-leak-kinds=all python3 TestScripts/py/envPrint.py  2>&1 | awk '/^{/{flag=1} flag; /}$/ {flag=0}' > python.supp && cat python.supp

##############################################################################33


testBase: $(TARGET_BASE)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_BASE)

testVal: $(TARGET_STRESSVALG)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--log-file=$(VALGRIND_OUTPUT) \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_STRESSVALG)

testStress: $(TARGET_STRESSTEST)
	@rm -rf $(VALGRIND_OUTPUT)
	@./$(TARGET_STRESSTEST)

testChildren: $(TARGET_STRESSCHILDREN) 
	@valgrind   \
	--quiet \
	--log-file=$(VALGRIND_OUTPUT) \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	--trace-children=yes \
	--track-origins=yes \
	--track-fds=yes	\
	./$(TARGET_STRESSCHILDREN)

test: testBase testVal testStress testChildren

clean:
	rm -rf $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG) $(TARGET_STRESSCHILDREN) vgcore* $(VALGRIND_OUTPUT)

fclean: clean


re: fclean all

.PHONY: clean fclean run test testBase testVal testStress testChildren re

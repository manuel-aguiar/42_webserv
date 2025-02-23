

# Compiler and flags
CXX 					= 	c++
CXXFLAGS 				= 	-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
TARGET_BASE 			= 	test.out

VALGRIND_OUTPUT 		= 	valgrind_output.log

# Paths
ROOT_PATH	=	$(shell git rev-parse --show-toplevel)
SRC_PATH	=	$(ROOT_PATH)/.
OBJ_PATH	=	$(ROOT_PATH)/_build/debug

# Sources

FUNC_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/Main/main.cpp

COMMON_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_Tests/Main/test1.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/Main/test2.cpp \
				\
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoRequest/TestProtoRequest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoRequest/TestProtoRequestHttpCgiGateway.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/TestProtoRequest/TestProtoRequestHandlers.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/Mocks/MockLogFile.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/Mocks/MockGlobals.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_Tests/Mocks/MockClock.cpp \
									\
				$(SRC_PATH)/Webserv/CgiModule/Module/Module.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModule.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModuleExternal.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModuleInternal.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModuleGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModuleRecycle.cpp \
				$(SRC_PATH)/Webserv/CgiModule/ImplModule/ImplModuleReturn.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Request/Request.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Request/RequestExternal.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalReq/InternalReq.cpp \
				$(SRC_PATH)/Webserv/CgiModule/InternalReq/InternalReqGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Worker/Worker.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Worker/WorkerEvents.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Worker/WorkerExecute.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Worker/WorkerHelpers.cpp \
				$(SRC_PATH)/Webserv/CgiModule/Worker/WorkerToImplModule.cpp \
				$(SRC_PATH)/Webserv/CgiModule/HeaderData/HeaderData.cpp \
				\
				$(SRC_PATH)/Webserv/SignalHandler/SignalHandler.cpp \
				$(SRC_PATH)/Webserv/TimerTracker/Timer/Timer.cpp \
				$(SRC_PATH)/Webserv/Events/Manager/Manager.cpp \
				$(SRC_PATH)/Webserv/Events/FdTracker/FdTracker.cpp \
				$(SRC_PATH)/Webserv/Events/Subscription/Subscription.cpp \
				$(SRC_PATH)/Webserv/Events/Subscription/SubscriptionGetSetAccess.cpp \
				$(SRC_PATH)/Webserv/Events/InternalSub/InternalSub.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/Buffer/BaseBuffer.cpp \
				$(SRC_PATH)/Webserv/GenericUtils/BufferView/BufferView.cpp

#################################################################
OBJS_COMMON = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(COMMON_SRCS))
OBJS_BASE = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(FUNC_SRCS))


##########################################################
DEPS_COMMON = $(OBJS_COMMON:.o=.d)
DEPS_BASE = $(OBJS_BASE:.o=.d)

###############################################################3

all: $(TARGET_BASE)
###################################################################

$(TARGET_BASE): $(OBJS_COMMON) $(OBJS_BASE)
	$(CXX) $(CXXFLAGS) -o $@ $^


#################################################################
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

################################################################################################
-include $(DEPS_COMMON) $(DEPS_BASE)

##chatgpt magic to generate suppresion files
#valgrind --gen-suppressions=all --leak-check=full --show-leak-kinds=all python3 TestScripts/py/envPrint.py  2>&1 | awk '/^{/{flag=1} flag; /}$/ {flag=0}' > python.supp && cat python.supp

##############################################################################33



test: $(TARGET_BASE)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_BASE)

debug: test

clean:
	rm -rf $(TARGET_BASE) vgcore* $(VALGRIND_OUTPUT)

fclean: clean


re: fclean all

.PHONY: clean fclean run test testBase testVal testStress testChildren re

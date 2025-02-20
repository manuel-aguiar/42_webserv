

# Compiler and flags
CXX 					= 	c++
CXXFLAGS 				= 	-Wall -Wextra -Werror -g -std=c++98 -MMD -MP
TARGET_BASE 			= 	functional_test.out
TARGET_STRESSTEST 		= 	stress_test.out
TARGET_STRESSVALG		= 	val_stress.out
TARGET_STRESSCHILDREN 	= 	children_stress.out

VALGRIND_OUTPUT 		= 	valgrind_output.log

# Paths
ROOT_PATH	=	$(shell git rev-parse --show-toplevel)
SRC_PATH	=	$(ROOT_PATH)/.
OBJ_PATH	=	$(ROOT_PATH)/_build/debug

# Sources

FUNC_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/main_FunctionalTest.cpp

STRESS_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/main_StressTest.cpp

STRESSVALG_SRCS = $(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/main_StressValgrind.cpp

STRESSCHILDREN = $(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/main_StressChildren.cpp

COMMON_SRCS = 	$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/test1.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/test2.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/CgiStressTest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Main/CgiStressTest_ExpectedOutput.cpp \
				\
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/TestProtoRequest/TestProtoRequest.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/TestProtoRequest/TestProtoRequestHttpCgiGateway.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/TestProtoRequest/TestProtoRequestHandlers.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Mocks/MockLogFile.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Mocks/MockGlobals.cpp \
				$(SRC_PATH)/Webserv/CgiModule/_OldTests/Mocks/MockClock.cpp \
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
				$(SRC_PATH)/Webserv/CgiModule/HeaderParser/HeaderParser.cpp \
				\
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


basetest: $(TARGET_BASE)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_BASE)

valgrind: $(TARGET_STRESSVALG)
	@rm -rf $(VALGRIND_OUTPUT)
	@valgrind   \
	--log-file=$(VALGRIND_OUTPUT) \
	--quiet \
	--leak-check=full \
	--errors-for-leak-kinds=all \
	--show-leak-kinds=all \
	./$(TARGET_STRESSVALG)

stresstest: $(TARGET_STRESSTEST)
	@rm -rf $(VALGRIND_OUTPUT)
	@./$(TARGET_STRESSTEST)

children: $(TARGET_STRESSCHILDREN) 
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

test: basetest valgrind stresstest children

clean:
	rm -rf $(TARGET_BASE) $(TARGET_STRESSTEST) $(TARGET_STRESSVALG) $(TARGET_STRESSCHILDREN) vgcore* $(VALGRIND_OUTPUT)

fclean: clean


re: fclean all

.PHONY: clean fclean run test testBase testVal testStress testChildren re

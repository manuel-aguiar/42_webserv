

# Compiler and flags
TARGET_TEST				=		test.out
TARGET_STRESS_TEST		=		stresstest.out
TARGET_MULTI_SERVER		=		multiserver.out
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -g -std=c++98 -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build/debug

SUBJECT_PATH			=		$(SRC_PATH)/Toolkit/TestHelpers/_Demo
# Sources
SRCS_COMMON				=  		$(SRC_PATH)/Webserv/Connections/_Tests/testMonitor.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testInternalListener.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testListener.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testConnection.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testManager.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testStressTest.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testMultiServer.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/testValgrindStress.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/_TestDependencies.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/_ServerProtos.cpp \
								$(SRC_PATH)/Webserv/Connections/_Tests/_ClientProtos.cpp \
								\
								$(SRC_PATH)/Webserv/Connections/Manager/Manager.cpp \
								$(SRC_PATH)/Webserv/Connections/Connection/Connection.cpp \
								$(SRC_PATH)/Webserv/Connections/Connection/ConnectionGetSetAccess.cpp \
								$(SRC_PATH)/Webserv/Connections/Connection/ConnectionEvents.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManager.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManagerInitShutdown.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalConn/InternalConn.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListener.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListenerOpenClose.cpp \
								$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListenerAccept.cpp \
								$(SRC_PATH)/Webserv/Connections/Monitor/Monitor.cpp \
								$(SRC_PATH)/Webserv/Connections/ConnInfo/ConnInfo.cpp \
								$(SRC_PATH)/Webserv/Connections/Listener/Listener.cpp \
								\
								$(SRC_PATH)/Webserv/Events/Manager/Manager.cpp \
								$(SRC_PATH)/Webserv/Events/FdTracker/FdTracker.cpp \
								$(SRC_PATH)/Webserv/Events/Subscription/Subscription.cpp \
								$(SRC_PATH)/Webserv/Events/Subscription/SubscriptionGetSetAccess.cpp \
								$(SRC_PATH)/Webserv/Events/InternalSub/InternalSub.cpp \
								\
								$(SRC_PATH)/Webserv/ServerContext/ServerContext.cpp \
								$(SRC_PATH)/Webserv/Globals/_Mock/MockLogFile.cpp \
								$(SRC_PATH)/Webserv/Globals/_Mock/MockGlobals.cpp \
								$(SRC_PATH)/Webserv/Globals/_Mock/MockClock.cpp \
								$(SRC_PATH)/Webserv/TimerTracker/Timer/Timer.cpp \
																	\
								$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp \
								\
								$(SRC_PATH)/Toolkit/ThreadPool/TaskQueue/TaskQueue.cpp \
								$(SRC_PATH)/Toolkit/ThreadPool/ThreadWorker/ThreadWorker.cpp \
								$(SRC_PATH)/Toolkit/ThreadPool/ThreadPoolHeap/ThreadPoolHeap.cpp \
								$(SRC_PATH)/Toolkit/ThreadPool/_ThreadPoolImpl/ThreadPoolImpl.cpp \
								\
								$(SRC_PATH)/Toolkit/MemoryPool/_SlabImpl/SlabImpl.cpp

SRCS_TEST				=		$(SRC_PATH)/Webserv/Connections/_Tests/main.cpp
SRCS_MULTI_SERVER		=		$(SRC_PATH)/Webserv/Connections/_Tests/mainMultiServer.cpp
SRCS_STRESS_TEST		=		$(SRC_PATH)/Webserv/Connections/_Tests/mainStressTest.cpp

OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))
OBJS_MULTI_SERVER		=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_MULTI_SERVER))
OBJS_STRESS_TEST		=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_STRESS_TEST))

DEPS_COMMON				=		$(OBJS_COMMON:.o=.d)
DEPS_TEST				=		$(OBJS_TEST:.o=.d)
DEPS_MULTI_SERVER		=		$(OBJS_MULTI_SERVER:.o=.d)
DEPS_STRESS_TEST		=		$(OBJS_STRESS_TEST:.o=.d)

#####################################################

all: $(TARGET_TEST) $(TARGET_STRESS_TEST) $(TARGET_MULTI_SERVER)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST) -lpthread

$(TARGET_STRESS_TEST): $(OBJS_COMMON) $(OBJS_STRESS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_STRESS_TEST) -o $(TARGET_STRESS_TEST) -lpthread

$(TARGET_MULTI_SERVER): $(OBJS_COMMON) $(OBJS_MULTI_SERVER)
	$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_MULTI_SERVER) -o $(TARGET_MULTI_SERVER) -lpthread

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_TEST) $(DEPS_STRESS_TEST) $(DEPS_MULTI_SERVER)

#####################################################

all: $(NAME)

test: all
	@valgrind  --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all  ./$(TARGET_TEST)

debug: test

clean:
	rm -rf vgcore*

fclean: clean
	rm -rf $(TARGET_TEST) $(TARGET_STRESS_TEST) $(OBJ_PATH)

re: fclean all

.PHONY: clean fclean run

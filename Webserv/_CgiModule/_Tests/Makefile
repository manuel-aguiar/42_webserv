
# 
# This is just a Makefile to compile tests
# It must be adjusted acoording to dependencies of tests themselves
#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
TARGET = test.out

SRCS = $(wildcard *.cpp) \
											\
		../CgiModule/CgiModule.cpp \
		../CgiModule/CgiModuleExternal.cpp \
		../CgiModule/CgiModuleInternal.cpp \
		../CgiModule/CgiModuleGetSetAccess.cpp \
		../InternalCgiRequestData/InternalCgiRequestData.cpp \
		../InternalCgiWorker/InternalCgiWorker.cpp \
		../InternalCgiWorker/InternalCgiWorkerEvents.cpp \
		../InternalCgiWorker/InternalCgiWorkerExecute.cpp \
		../InternalCgiWorker/InternalCgiWorkerUtils.cpp \
											\
		../CgiRequestData/CgiRequestData.cpp \
		../CgiRequestData/CgiRequestDataGetSetAccess.cpp	\
		\
		../../ServerManager/EventManager/EventManager.cpp \
											\
		../../Globals/_Mock/MockLogFile.cpp \
		../../Globals/_Mock/MockGlobals.cpp \
		../../Globals/_Mock/MockClock.cpp \
		../../Globals/SignalHandler/SignalHandler.cpp \
											\
		../../HttpModule/_Mock/MockHttpModule.cpp \
		../../HttpModule/_Mock/MockHttpManager.cpp \
											\
		../../GenericUtils/FileDescriptor/FileDescriptor.cpp \
		../../GenericUtils/StringUtils/StringUtils.cpp \
		../../GenericUtils/Validation/Validation.cpp \
		../../Event/Event.cpp \
		../../Event/EventGetSetAccess.cpp \
		TestProtoConnections/A_ProtoConn.cpp \
		TestProtoConnections/B_ProtoConn.cpp \
		TestProtoConnections/C_ProtoConn.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET) vgcore*

fclean: clean
	rm -rf $(TARGET)

re: fclean run

.PHONY: clean fclean run

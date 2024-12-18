
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
		../CgiModule/CgiModuleGetSetAccess.cpp \
		../CgiModule/CgiModuleManagedRequestData.cpp \
		../CgiLiveRequest/CgiLiveRequest.cpp \
		../CgiLiveRequest/CgiLiveRequestEvents.cpp \
		../CgiLiveRequest/CgiLiveRequestExecute.cpp \
		../CgiLiveRequest/CgiLiveRequestUtils.cpp \
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
		../../Event/EventGetSetAccess.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

run: $(TARGET)
	valgrind --track-fds=yes ./$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

fclean: clean
	$(TARGET)

re: fclean all

.PHONY: clean fclean run

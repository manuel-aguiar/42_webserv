###########################
## Main Webserv Makefile ##
###########################

NAME 					= 		webserv
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -std=c++98 -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/Webserv/.
OBJ_PATH				=		$(ROOT_PATH)/_build

# Sources

SRCS 					=																	\
																							\
		$(SRC_PATH)/main.cpp																\
																							\
																							\
		$(SRC_PATH)/TimerTracker/Timer/Timer.cpp											\
																							\
																							\
		$(SRC_PATH)/SignalHandler/SignalHandler.cpp											\
																							\
																							\
		$(SRC_PATH)/CgiModule/Module/Module.cpp												\
		$(SRC_PATH)/CgiModule/Worker/WorkerHelpers.cpp										\
		$(SRC_PATH)/CgiModule/Worker/WorkerEvents.cpp										\
		$(SRC_PATH)/CgiModule/Worker/WorkerExecute.cpp										\
		$(SRC_PATH)/CgiModule/Worker/WorkerToImplModule.cpp									\
		$(SRC_PATH)/CgiModule/Worker/Worker.cpp												\
		$(SRC_PATH)/CgiModule/InternalReq/InternalReq.cpp									\
		$(SRC_PATH)/CgiModule/InternalReq/InternalReqGetSetAccess.cpp						\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModuleRecycle.cpp								\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModuleGetSetAccess.cpp							\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModuleReturn.cpp								\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModule.cpp										\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModuleExternal.cpp								\
		$(SRC_PATH)/CgiModule/ImplModule/ImplModuleInternal.cpp								\
		$(SRC_PATH)/CgiModule/Request/Request.cpp											\
		$(SRC_PATH)/CgiModule/Request/RequestExternal.cpp									\
																							\
																							\
		$(SRC_PATH)/Events/InternalSub/InternalSub.cpp										\
		$(SRC_PATH)/Events/FdTracker/FdTracker.cpp											\
		$(SRC_PATH)/Events/Manager/Manager.cpp												\
		$(SRC_PATH)/Events/Subscription/SubscriptionGetSetAccess.cpp						\
		$(SRC_PATH)/Events/Subscription/Subscription.cpp									\
																							\
																							\
		$(SRC_PATH)/ServerContext/ServerContext.cpp											\
																							\
																							\
		$(SRC_PATH)/HttpModule/Module/Module.cpp											\
		$(SRC_PATH)/HttpModule/Module/ModuleInitClose.cpp									\
		$(SRC_PATH)/HttpModule/Response/Response.cpp										\
		$(SRC_PATH)/HttpModule/HttpRequest/HttpRequest.cpp									\
		$(SRC_PATH)/HttpModule/HttpRequest/parsers/http_parse_header_line.cpp				\
		$(SRC_PATH)/HttpModule/HttpRequest/parsers/http_parse_message_body.cpp				\
		$(SRC_PATH)/HttpModule/HttpRequest/parsers/http_parse_request_line.cpp				\
		$(SRC_PATH)/HttpModule/Connection/Connection.cpp									\
		$(SRC_PATH)/HttpModule/HttpDefinitions.cpp											\
		$(SRC_PATH)/HttpModule/Request/Request.cpp											\
																							\
																							\
		$(SRC_PATH)/ServerConfig/ServerBlock/ServerBlock.cpp								\
		$(SRC_PATH)/ServerConfig/ServerLocation/ServerLocation.cpp							\
		$(SRC_PATH)/ServerConfig/DefaultConfig/DefaultConfig.cpp							\
		$(SRC_PATH)/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp						\
		$(SRC_PATH)/ServerConfig/ServerConfig/ServerConfig.cpp								\
		$(SRC_PATH)/ServerConfig/BlockFinder/BlockFinder.cpp								\
																							\
																							\
		$(SRC_PATH)/Globals/Globals.cpp														\
		$(SRC_PATH)/Globals/Clock/Clock.cpp													\
		$(SRC_PATH)/Globals/LogFile/LogFile.cpp												\
																							\
																							\
		$(SRC_PATH)/Connections/InternalConn/InternalConn.cpp								\
		$(SRC_PATH)/Connections/ConnInfo/ConnInfo.cpp										\
		$(SRC_PATH)/Connections/Monitor/Monitor.cpp											\
		$(SRC_PATH)/Connections/InternalManager/InternalManagerInitShutdown.cpp				\
		$(SRC_PATH)/Connections/InternalManager/InternalManager.cpp							\
		$(SRC_PATH)/Connections/Connection/ConnectionGetSetAccess.cpp						\
		$(SRC_PATH)/Connections/Connection/ConnectionEvents.cpp								\
		$(SRC_PATH)/Connections/Connection/Connection.cpp									\
		$(SRC_PATH)/Connections/Manager/Manager.cpp											\
		$(SRC_PATH)/Connections/InternalListener/InternalListener.cpp						\
		$(SRC_PATH)/Connections/InternalListener/InternalListenerAccept.cpp					\
		$(SRC_PATH)/Connections/InternalListener/InternalListenerOpenClose.cpp				\
		$(SRC_PATH)/Connections/Listener/Listener.cpp										\
																							\
																							\
		$(SRC_PATH)/GenericUtils/FileDescriptor/FileDescriptor.cpp							\
		$(SRC_PATH)/GenericUtils/StringUtils/StringUtils.cpp								\
		$(SRC_PATH)/GenericUtils/Validation/Validation.cpp									

######################################################

OBJS               		=       $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))

DEPS               		=       $(OBJS:.o=.d)

#####################################################

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS)  $(OBJS) -o $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

#####################################################

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re									
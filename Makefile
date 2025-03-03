###########################
## Main Webserv Makefile ##
###########################

NAME 					= 		webserv
CXX						=		c++
CXXFLAGS				=		-Wall -Wextra -Werror -Wpedantic -std=c++98 -MMD -MP

DEBUG_FLAGS				=		-g -DDEBUG
PROD_FLAGS				=		-O3 -DNDEBUG

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)
BUILD_DIR               =       $(ROOT_PATH)/_build

# Build type (default to prod)
BUILD                   ?=      prod
ifeq ($(BUILD), debug)
    OBJ_PATH            =       $(BUILD_DIR)/debug
    CXXFLAGS           +=       $(DEBUG_FLAGS)
else
    OBJ_PATH            =       $(BUILD_DIR)/prod
    CXXFLAGS           +=       $(PROD_FLAGS)
endif

# Sources
SRCS 					=																			\
																									\
		$(SRC_PATH)/Webserv/main.cpp																\
		$(SRC_PATH)/Webserv/Run/WebservRun.cpp														\
																									\
																									\
		$(SRC_PATH)/Webserv/TimerTracker/Timer/Timer.cpp											\
																									\
																									\
		$(SRC_PATH)/Webserv/SignalHandler/SignalHandler.cpp											\
																									\
																									\
		$(SRC_PATH)/Webserv/Events/InternalSub/InternalSub.cpp										\
		$(SRC_PATH)/Webserv/Events/FdTracker/FdTracker.cpp											\
		$(SRC_PATH)/Webserv/Events/Manager/Manager.cpp												\
		$(SRC_PATH)/Webserv/Events/Subscription/SubscriptionGetSetAccess.cpp						\
		$(SRC_PATH)/Webserv/Events/Subscription/Subscription.cpp									\
																									\
																									\
		$(SRC_PATH)/Webserv/ServerContext/ServerContext.cpp											\
																									\
																									\
								\
		$(SRC_PATH)/Webserv/HttpModule/HttpConnection/HttpConnection.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpConnection/HttpConnection_ReadWrite.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpModule/HttpModule.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpModule/HttpModule_InitClose.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_header_line.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_request_line.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_BodyChunked.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_BodyRegular.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_BodyMultipart.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_External.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ErrorCodes.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_Redirect.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_DirectoryListing.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FileUpload.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FileDownload.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FillFunctions.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_CurrentDate.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ValidateHeaders.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ValidateAcceptType.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_Others.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_AssembleTargetPath.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FindLocation.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpTransaction/HttpTransaction.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpDefinitions.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/CgiHandlers.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_Validate.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_FillFunctions.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_FillErrorMessage.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_External.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_Helpers.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_onCallbacks.cpp \
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_ProcessHttpBody.cpp \
		\
		$(SRC_PATH)/Webserv/HttpModule/HttpCgiInterface/HttpCgiInterface.cpp \
																									\
																									\
		$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp								\
		$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp							\
		$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp							\
		$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp						\
		$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp								\
		$(SRC_PATH)/Webserv/ServerConfig/BlockFinder/BlockFinder.cpp								\
																									\
																									\
		$(SRC_PATH)/Webserv/Globals/Globals.cpp														\
		$(SRC_PATH)/Webserv/Globals/Clock/Clock.cpp													\
		$(SRC_PATH)/Webserv/Globals/LogFile/LogFile.cpp												\
																									\
																									\
		$(SRC_PATH)/Webserv/Connections/InternalConn/InternalConn.cpp								\
		$(SRC_PATH)/Webserv/Connections/ConnInfo/ConnInfo.cpp										\
		$(SRC_PATH)/Webserv/Connections/Monitor/Monitor.cpp											\
		$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManagerInitShutdown.cpp				\
		$(SRC_PATH)/Webserv/Connections/InternalManager/InternalManager.cpp							\
		$(SRC_PATH)/Webserv/Connections/Connection/ConnectionGetSetAccess.cpp						\
		$(SRC_PATH)/Webserv/Connections/Connection/ConnectionEvents.cpp								\
		$(SRC_PATH)/Webserv/Connections/Connection/Connection.cpp									\
		$(SRC_PATH)/Webserv/Connections/Manager/Manager.cpp											\
		$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListener.cpp						\
		$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListenerAccept.cpp					\
		$(SRC_PATH)/Webserv/Connections/InternalListener/InternalListenerOpenClose.cpp				\
		$(SRC_PATH)/Webserv/Connections/Listener/Listener.cpp										\
																									\
																									\
		$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp							\
		$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp								\
		$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp									\
		$(SRC_PATH)/Webserv/GenericUtils/Files/File.cpp												\
		$(SRC_PATH)/Webserv/GenericUtils/Files/FilesUtils.cpp										\
		$(SRC_PATH)/Webserv/GenericUtils/Buffer/BaseBuffer.cpp										\
		$(SRC_PATH)/Webserv/GenericUtils/BufferView/BufferView.cpp									\
		\
		$(SRC_PATH)/Webserv/CgiModule/CgiNamespace.cpp \
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
	rm -rf $(OBJ_PATH) *vgcore*

fclean: clean
	rm -rf $(NAME) $(BUILD_DIR)

re: fclean all

debug:
	@$(MAKE) BUILD=debug

prod:
	@$(MAKE) BUILD=prod

.PHONY: clean fclean re	debug prod								
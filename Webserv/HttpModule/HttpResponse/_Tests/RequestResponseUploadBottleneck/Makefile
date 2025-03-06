# Makefile

# TARGET_DEBUG 			=		debug_Http::Request.out
TARGET_TEST 			=		test.out
CXX						=		c++
CXXFLAGS 				=		-g -std=c++98 -Wpedantic -Wall -Wextra -Werror -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build/debug

SRCS_TEST				=		$(SRC_PATH)/Webserv/HttpModule/HttpResponse/_Tests/RequestResponseUploadBottleneck/main.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/_Tests/RequestResponseUpload/texts.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/_Tests/RequestResponseUploadBottleneck/MockHttpResponse_External.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/_Tests/RequestResponseUploadBottleneck/MockFile.cpp
								

# SRCS_DEBUG				=		$(SRC_PATH)/Webserv/ServerConfig/_Tests/ServerConfigTests.cpp \

SRCS_COMMON				=		$(SRC_PATH)/Webserv/HttpModule/HttpConnection/HttpConnection.cpp \
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
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ResolveRequestData.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ValidateAcceptType.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_Redirect.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FileUpload.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FindLocation.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FileDownload.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ErrorCodes.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_DirectoryListing.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FillDirectoryListing.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_FillFunctions.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_CurrentDate.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_AssembleTargetPath.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_Others.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_AddHeaders.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_Etag.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpTransaction/HttpTransaction.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpDefinitions.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_External.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_Validate.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_FillFunctions.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_FillErrorMessage.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_onCallbacks.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_ProcessHttpBody.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/HttpCgiResponse_Helpers.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiResponse/CgiHandlers.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpCgiInterface/HttpCgiInterface.cpp \
								\
								$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp \
							 	$(SRC_PATH)/Webserv/ServerContext/ServerContext.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/BlockFinder/BlockFinder.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/FileDescriptor/FileDescriptor.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Files/FilesUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Buffer/BaseBuffer.cpp										\
								$(SRC_PATH)/Webserv/GenericUtils/Buffer/HeapBuffer.cpp										\
								$(SRC_PATH)/Webserv/GenericUtils/BufferView/BufferView.cpp									\
										\
								$(SRC_PATH)/Webserv/TimerTracker/Timer/Timer.cpp											\
								\
								$(SRC_PATH)/Webserv/Globals/Globals.cpp														\
								$(SRC_PATH)/Webserv/Globals/Clock/Clock.cpp													\
								$(SRC_PATH)/Webserv/Globals/LogFile/LogFile.cpp												\
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
								$(SRC_PATH)/Webserv/Events/InternalSub/InternalSub.cpp										\
								$(SRC_PATH)/Webserv/Events/FdTracker/FdTracker.cpp											\
								$(SRC_PATH)/Webserv/Events/Manager/Manager.cpp												\
								$(SRC_PATH)/Webserv/Events/Subscription/SubscriptionGetSetAccess.cpp						\
								$(SRC_PATH)/Webserv/Events/Subscription/Subscription.cpp									\
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
								\
								$(SRC_PATH)/Webserv/SignalHandler/SignalHandler.cpp \

OBJS_COMMON				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_COMMON))
OBJS_DEBUG				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_DEBUG))
OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

DEPS_COMMON 			= 		$(OBJS_COMMON:.o=.d)
DEPS_DEBUG 				= 		$(OBJS_DEBUG:.o=.d)
DEPS_TEST 				= 		$(OBJS_TEST:.o=.d)

#=== COLORS ===#

RED =		\033[0;31m
GREEN =		\033[0;92m
YELLOW =	\033[93m
BLUE =		\033[0;34m
MAGENTA =	\033[0;35m
CYAN =		\033[96m
RESET =		\033[0m

TAG =		$(CYAN)[$(NAME)]$(RESET)

#=== RULES ===#

all: $(TARGET_TEST) # $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJS_COMMON) $(OBJS_DEBUG)
	@echo "$(CYAN)[$(TARGET_DEBUG)]$(RESET) Making Program"
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_DEBUG) -o $(TARGET_DEBUG)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_COMMON) $(DEPS_DEBUG) $(DEPS_TEST)

clean:
	@rm -rf *vgcore*

fclean: clean
	@echo "$(TAG) Full clean."
	@rm -f $(TARGET_TEST)

test: $(TARGET_TEST)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_TEST)

debug: $(TARGET_DEBUG)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_DEBUG)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re
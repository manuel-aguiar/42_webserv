
# Makefile

# TARGET_DEBUG 			=		debug_httpRequest.out
TARGET_TEST 			=		test_httpRequest.out
CXX						=		c++
CXXFLAGS 				=		-g -std=c++98 -Wall -Wextra -Werror -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build

SRCS_TEST				=		$(SRC_PATH)/Webserv/HttpModule/_Tests/CompileAll/main.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpConnection/HttpConnection.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpModule/HttpModule.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpModule/HttpModule_InitClose.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_header_line.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_message_body.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpRequest/HttpRequest_parse_request_line.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_ErrorCodes.cpp \
								$(SRC_PATH)/Webserv/HttpModule/HttpResponse/HttpResponse_DirectoryListing.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpTransaction/HttpTransaction.cpp \
								\
								$(SRC_PATH)/Webserv/HttpModule/HttpDefinitions.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp \
							 	$(SRC_PATH)/Webserv/ServerContext/ServerContext.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
							 	$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp \
								$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Files/FilesUtils.cpp \
								$(SRC_PATH)/Webserv/GenericUtils/Files/File.cpp
								

								
# Delete the 4 above


OBJS_TEST				=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS_TEST))

DEPS_TEST 				= 		$(OBJS_TEST:.o=.d)



#=== RULES ===#

all: $(TARGET_TEST)

$(TARGET_TEST): $(OBJS_COMMON) $(OBJS_TEST)
	@$(CXX) $(CXXFLAGS) $(OBJS_COMMON) $(OBJS_TEST) -o $(TARGET_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS_TEST)

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

test: $(TARGET_TEST)
	@valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all ./$(TARGET_TEST)

re: fclean all
	@echo "$(TAG) Recompiling."

.PHONY: all clean fclean re

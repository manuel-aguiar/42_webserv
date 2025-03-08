#
# This is just a Makefile to compile tests
# It must be adjusted according to dependencies of tests themselves
#

# Compiler and flags
TARGET_DEBUG			= 		test.out
CXX						= 		c++
CXXFLAGS				= 		-Wall -Wextra -Werror -std=c++98 -g -MMD -MP

# Paths
ROOT_PATH				=		$(shell git rev-parse --show-toplevel)
SRC_PATH				=		$(ROOT_PATH)/.
OBJ_PATH				=		$(ROOT_PATH)/_build/debug


SRCS					=	$(SRC_PATH)/Webserv/ServerConfig/_IntegrationTests/BlockFinder-ServerBlock/test.cpp \
							\
							$(SRC_PATH)/Webserv/ServerConfig/BlockFinder/BlockFinder.cpp \
							$(SRC_PATH)/Webserv/ServerConfig/ServerBlock/ServerBlock.cpp \
							$(SRC_PATH)/Webserv/ServerConfig/ServerLocation/ServerLocation.cpp \
							$(SRC_PATH)/Webserv/ServerConfig/DefaultConfig/DefaultConfig.cpp \
							$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfig.cpp \
							$(SRC_PATH)/Webserv/ServerConfig/ServerConfig/ServerConfigDNSLookup.cpp \
							$(SRC_PATH)/Webserv/GenericUtils/StringUtils/StringUtils.cpp \
							$(SRC_PATH)/Webserv/GenericUtils/Files/FilesUtils.cpp \
							$(SRC_PATH)/Webserv/GenericUtils/BufferView/BufferView.cpp \
							$(SRC_PATH)/Webserv/GenericUtils/Validation/Validation.cpp

OBJS					=		$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRCS))
DEPS					=		$(OBJS:.o=.d)

# Rules
all: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET_DEBUG)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf *vgcore*

fclean: clean
	rm -rf $(TARGET_DEBUG)

re: fclean all

debug: $(TARGET_DEBUG)
	@valgrind --quiet --leak-check=full --show-leak-kinds=all ./$(TARGET_DEBUG)

run: $(TARGET_DEBUG)
	@echo "\nRunning integration tests with valgrind...\n"
	@valgrind --quiet --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(TARGET_DEBUG)

test: debug


.PHONY: all clean fclean re run test debug

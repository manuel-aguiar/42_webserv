#include <gtest/gtest.h>
#include "ServerConfigParser.h"  // Replace with your actual header file
#include <fstream>
#include <stdexcept>

// Test with valid configuration
TEST(ServerConfigTests, ValidConfig) {
    std::ifstream valid_config("tests/utils/TestFiles/valid_config.txt");
    ServerConfigParser parser;
    
    ASSERT_NO_THROW(parser.parse(valid_config));
    EXPECT_TRUE(parser.isValid());
}

// Test with missing server_name (invalid config)
TEST(ServerConfigTests, MissingServerName) {
    std::ifstream invalid_config("tests/utils/TestFiles/invalid_config_1.txt");
    ServerConfigParser parser;

    EXPECT_THROW(parser.parse(invalid_config), std::invalid_argument);
}

// Test with invalid listen directive
TEST(ServerConfigTests, InvalidListen) {
    std::ifstream invalid_config("tests/utils/TestFiles/invalid_config_2.txt");
    ServerConfigParser parser;

    EXPECT_THROW(parser.parse(invalid_config), std::invalid_argument);
}
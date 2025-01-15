// # include "../RequestParser.hpp"
// # include "TestDependencies.hpp"
// # include <iostream>

// int main(void)
// {

// 	try
// 	{
// 		std::cout << "	PASS\n";
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << "	FAILED: " << e.what() << '\n';
// 	}

// 	return (0);
// }


#include <iostream>
#include <sstream> // For std::istringstream
#include <string>

int main() {
    // Input string to parse
    std::string input = "GET /index.html HTTP/1.1";

    // Create a string stream from the input
    std::istringstream stream(input);

    // Variables to hold parsed tokens
    std::string method, uri, version;

    // Parse the input using stream extraction
    if (stream >> method >> uri >> version) {
        // Successfully parsed all three components
        std::cout << "Method: " << method << std::endl;
        std::cout << "URI: " << uri << std::endl;
        std::cout << "Version: " << version << std::endl;
    } else {
        // Parsing failed
        std::cerr << "Failed to parse input!" << std::endl;
    }

    // Demonstrate error checking
    if (stream) {
        std::cout << "Stream is still valid after parsing." << std::endl;
    } else {
        std::cout << "Stream is invalid after parsing." << std::endl;
    }

    return 0;
}

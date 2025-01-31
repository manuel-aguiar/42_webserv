#include "../HttpRequest.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>

int main() {
    std::cout << "Starting HTTP Request Parser Tests...\n";

    testRequestLineParsing();

    std::cout << "\nAll tests completed!\n";
    return 0;
}

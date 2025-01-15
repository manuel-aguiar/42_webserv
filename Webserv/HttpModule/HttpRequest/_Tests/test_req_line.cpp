#include "../HttpRequest.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// Helper function to print request details
void printRequest(const std::string& method, const std::string& uri, const std::string& version, const std::string& headers) {
    std::cout << "┌──────────── Request Details ────────────┐" << std::endl;
    std::cout << "│ Method:  " << std::left << std::setw(31) << method << "│" << std::endl;
    std::cout << "│ URI:     " << std::left << std::setw(31) << uri << "│" << std::endl;
    std::cout << "│ Version: " << std::left << std::setw(31) << version << "│" << std::endl;
    std::cout << "│ Headers: " << std::left << std::setw(31) << headers << "│" << std::endl;
    std::cout << "└─────────────────────────────────────────┘" << std::endl;
}

void testRequestLineParsing() {
    std::cout << "\n=== Testing Request Line Parsing ===\n";

    {
        std::cout << "  Test1: Valid request line (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /index.html HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/index.html", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::OK)
                throw std::runtime_error("Expected status OK, got: " + std::to_string(status));
            if (request.getMethod() != "GET")
                throw std::runtime_error("Expected method GET, got: " + request.getMethod());
            if (request.getUri() != "/index.html")
                throw std::runtime_error("Expected URI /index.html, got: " + request.getUri());
            if (request.getHttpVersion() != "HTTP/1.1")
                throw std::runtime_error("Expected version HTTP/1.1, got: " + request.getHttpVersion());

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test2: Invalid method (expected: METHOD_NOT_ALLOWED):\n";
        try {
            const std::string raw_request =
                "INVALID /index.html HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("INVALID", "/index.html", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::METHOD_NOT_ALLOWED)
                throw std::runtime_error("Expected status METHOD_NOT_ALLOWED, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test3: URI too long (expected: URI_TOO_LONG):\n";
        try {
            std::string long_uri = "/";
            for (int i = 0; i < 2048; ++i) {
                long_uri += "a";
            }

            const std::string raw_request =
                "GET " + long_uri + " HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "(URI length: " + std::to_string(long_uri.length()) + ")", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::URI_TOO_LONG)
                throw std::runtime_error("Expected status URI_TOO_LONG, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test4: Missing components (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request = "GET /index.html\r\n\r\n";

            printRequest("GET", "/index.html", "(missing)", "(empty)");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test5: Invalid HTTP version (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "GET /index.html HTTP/2.0\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/index.html", "HTTP/2.0", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test6: Multiple spaces between components (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "GET     /index.html     HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/index.html", "HTTP/1.1", "Host: localhost");
            std::cout << "  Note: Request contains multiple spaces between components\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test7: URI with query and fragment (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /search?q=test&page=1#section1 HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/search?q=test&page=1#section1", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::OK)
                throw std::runtime_error("Expected status OK, got: " + std::to_string(status));
            if (request.getUri() != "/search?q=test&page=1#section1")
                throw std::runtime_error("Expected URI /search?q=test&page=1#section1, got: " + request.getUri());

            const std::map<std::string, std::string>& components = request.getUriComponents();
            if (components.find("path") == components.end() || components.at("path") != "/search")
                throw std::runtime_error("Expected path /search, got: " + components.at("path"));
            if (components.find("q") == components.end() || components.at("q") != "test")
                throw std::runtime_error("Expected q=test, got: " + components.at("q"));
            if (components.find("page") == components.end() || components.at("page") != "1")
                throw std::runtime_error("Expected page=1, got: " + components.at("page"));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test8: Empty request line (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request = "\r\n\r\n";

            printRequest("(empty)", "(empty)", "(empty)", "(empty)");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != RequestStatus::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + std::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }
}

int main() {
    std::cout << "Starting HTTP Request Parser Tests...\n";

    testRequestLineParsing();

    std::cout << "\nAll tests completed!\n";
    return 0;
}

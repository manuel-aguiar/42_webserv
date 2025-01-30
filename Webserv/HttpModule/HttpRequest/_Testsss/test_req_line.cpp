#include "../HttpRequest.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
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

            if (status != Http::Status::OK)
                throw std::runtime_error("Expected status OK, got: " + StringUtils::to_string(status));
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

            if (status != Http::Status::METHOD_NOT_ALLOWED)
                throw std::runtime_error("Expected status METHOD_NOT_ALLOWED, got: " + StringUtils::to_string(status));

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

            printRequest("GET", "(URI length: " + StringUtils::to_string(long_uri.length()) + ")", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::URI_TOO_LONG)
                throw std::runtime_error("Expected status URI_TOO_LONG, got: " + StringUtils::to_string(status));

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

            if (status != Http::Status::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + StringUtils::to_string(status));

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

            if (status != Http::Status::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + StringUtils::to_string(status));

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

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + StringUtils::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test7: URI with no value in query (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /search?q=&r=v HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/search?q=&r=v", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::OK)
                throw std::runtime_error("Expected status OK, got: " + StringUtils::to_string(status));
            if (request.getUri() != "/search?q=&r=v")
                throw std::runtime_error("Expected URI /search?q=&r=v, got: " + request.getUri());

            std::map<std::string, std::string> components = request.getUriComponents();

            if (components.find("path") == components.end() || components.at("path") != "/search")
                throw std::runtime_error("Expected path /search, got: " + components.at("path"));
            if (components.find("q") == components.end() || components.at("q") != "")
                throw std::runtime_error("Expected q=, got: " + components.at("q"));
            if (components.find("r") == components.end() || components.at("r") != "v")
                throw std::runtime_error("Expected r=v, got: " + components.at("r"));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test8: URI with query and fragment (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /search?q=test&page=1#section1 HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/search?q=test&page=1#section1", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::OK)
                throw std::runtime_error("Expected status OK, got: " + StringUtils::to_string(status));
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
        std::cout << "  Test9: Empty request line (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request = "\r\n\r\n";

            printRequest("(empty)", "(empty)", "(empty)", "(empty)");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + StringUtils::to_string(status));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test10: URI with encoded characters (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /search%20path/file%2B1.html?name=%4A%6F%68%6E&title=Hello%20World%21 HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/search%20path/file%2B1.html?name=%4A%6F%68%6E&title=Hello%20World%21", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::OK)
                throw std::runtime_error("Expected status OK, got: " + StringUtils::to_string(status));

            const std::map<std::string, std::string>& components = request.getUriComponents();

            // Test path decoding
            if (components.find("path") == components.end() || components.at("path") != "/search path/file+1.html")
                throw std::runtime_error("Expected decoded path '/search path/file+1.html', got: " + components.at("path"));

            // Test query parameter decoding
            if (components.find("name") == components.end() || components.at("name") != "John")
                throw std::runtime_error("Expected decoded name 'John', got: " + components.at("name"));

            if (components.find("title") == components.end() || components.at("title") != "Hello World!")
                throw std::runtime_error("Expected decoded title 'Hello World!', got: " + components.at("title"));

            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }

        std::cout << "\n─────────────────────────────────────────\n" << std::endl;
    }

    {
        std::cout << "  Test11: URI with invalid encoding (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "GET /test%2path?param=%XX HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            printRequest("GET", "/test%2path?param=%XX", "HTTP/1.1", "Host: localhost");

            HttpRequest request;
            int status = request.parse(raw_request);

            if (status != Http::Status::BAD_REQUEST)
                throw std::runtime_error("Expected status BAD_REQUEST, got: " + StringUtils::to_string(status));

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

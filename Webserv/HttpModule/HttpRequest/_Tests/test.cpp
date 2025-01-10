#include "../HttpRequest.hpp"
#include <iostream>
#include <cassert>
#include <iomanip>

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

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getMethod() == "GET");
            assert(request.getUri() == "/index.html");
            assert(request.getHttpVersion() == "HTTP/1.1");

            printRequest("GET", "/index.html", "HTTP/1.1", "Host: localhost");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test2: Invalid method (expected: METHOD_NOT_ALLOWED):\n";
        try {
            const std::string raw_request =
                "INVALID /index.html HTTP/1.1\r\n"
                "Host: localhost\r\n\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::METHOD_NOT_ALLOWED);
            printRequest("INVALID", "/index.html", "HTTP/1.1", "Host: localhost");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
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

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::URI_TOO_LONG);
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }
}

void testHeaderParsing() {
    std::cout << "\n=== Testing Header Parsing ===\n";

    {
        std::cout << "  Test1: Valid headers (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET / HTTP/1.1\r\n"
                "Host: localhost:8080\r\n"
                "Accept: text/html\r\n"
                "User-Agent: curl/7.68.0\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getHeaders().at("Host") == "localhost:8080");
            assert(request.getHeaders().at("Accept") == "text/html");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test2: Missing required Host header (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "GET / HTTP/1.1\r\n"
                "Accept: text/html\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::BAD_REQUEST);
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test3: Headers too large (expected: HEADERS_TOO_LARGE):\n";
        try {
            std::string large_header_value(9000, 'x');
            const std::string raw_request =
                "GET / HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "X-Large-Header: " + large_header_value + "\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::HEADERS_TOO_LARGE);
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }
}

void testBodyParsing() {
    std::cout << "\n=== Testing Body Parsing ===\n";

    {
        std::cout << "  Test1: Valid POST request with body (expected: OK):\n";
        try {
            const std::string raw_request =
                "POST /submit HTTP/1.1\r\n"
                "Host: localhost:8080\r\n"
                "Content-Length: 11\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "\r\n"
                "name=John42";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getBody() == "name=John42");
            assert(request.getHeaders().at("Content-Length") == "11");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test2: Content-Length mismatch (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "POST /submit HTTP/1.1\r\n"
                "Host: localhost:8080\r\n"
                "Content-Length: 20\r\n"
                "\r\n"
                "name=John42";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::BAD_REQUEST);
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }
}

void testEncodedRequests() {
    std::cout << "\n=== Testing URL-Encoded Requests ===\n";

    {
        std::cout << "  Test1: URI with encoded characters (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /path%20with%20spaces/file%2Ename%2Etxt HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getUri() == "/path with spaces/file.name.txt");
            printRequest("GET", request.getUri(), "HTTP/1.1", "Host: localhost");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test2: URI with encoded special characters (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /test%3F%26%3D%23 HTTP/1.1\r\n"  // encoded "?&=#"
                "Host: localhost\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getUri() == "/test?&=#");
            printRequest("GET", request.getUri(), "HTTP/1.1", "Host: localhost");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test3: POST with URL-encoded form data (expected: OK):\n";
        try {
            const std::string raw_request =
                "POST /submit HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: 44\r\n"
                "\r\n"
                "user%20name=John%20Doe&email=john%40example.com";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getBody() == "user name=John Doe&email=john@example.com");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test4: Malformed URL encoding (expected: BAD_REQUEST):\n";
        try {
            const std::string raw_request =
                "GET /test%2 HTTP/1.1\r\n"  // Incomplete percent encoding
                "Host: localhost\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::BAD_REQUEST);
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }

    {
        std::cout << "  Test5: URI with query parameters (expected: OK):\n";
        try {
            const std::string raw_request =
                "GET /search?q=hello%20world&lang=en%2Dus HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "\r\n";

            HttpRequest request;
            int status = request.parse(raw_request);

            assert(status == RequestStatus::OK);
            assert(request.getUri() == "/search?q=hello world&lang=en-us");
            printRequest("GET", request.getUri(), "HTTP/1.1", "Host: localhost");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e) {
            std::cerr << "      FAILED: " << e.what() << '\n';
        }
    }
}

int main() {
    std::cout << "Starting HTTP Request Parser Tests...\n";

    testRequestLineParsing();
    // testHeaderParsing();
    // testBodyParsing();
    // testEncodedRequests();

    std::cout << "\nAll tests completed!\n";
    return 0;
}

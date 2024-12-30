#include "../HttpRequest.hpp"
#include <iostream>
#include <cassert>

void test_valid_simple_request() {
    std::cout << "\nTesting valid simple request..." << std::endl;

    const std::string raw_request =
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::OK);
    assert(request.getMethod() == "GET");
    assert(request.getUri() == "/index.html");
    assert(request.getHttpVersion() == "HTTP/1.1");
    assert(request.getHeaders().at("Host") == "localhost:8080");

    std::cout << "  PASS" << std::endl;
}

void test_invalid_request_line() {
    std::cout << "\nTesting invalid request line..." << std::endl;

    const std::string raw_request =
        "INVALID /index.html\r\n"  // Invalid method
        "Host: localhost:8080\r\n"
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::METHOD_NOT_ALLOWED);

    std::cout << "  PASS" << std::endl;
}

void test_post_request_with_body() {
    std::cout << "\nTesting POST request with body..." << std::endl;

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
    assert(request.getMethod() == "POST");
    assert(request.getBody() == "name=John42");
    assert(request.getHeaders().at("Content-Length") == "11");

    std::cout << "  PASS" << std::endl;
}

void test_malformed_headers() {
    std::cout << "\nTesting malformed headers..." << std::endl;

    const std::string raw_request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Invalid-Header-Line\r\n"  // Malformed header
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::BAD_REQUEST);

    std::cout << "  PASS" << std::endl;
}

void test_missing_host_header() {
    std::cout << "\nTesting missing Host header..." << std::endl;

    const std::string raw_request =
        "GET / HTTP/1.1\r\n"
        "Accept: */*\r\n"
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::BAD_REQUEST);

    std::cout << "  PASS" << std::endl;
}

void test_uri_too_long() {
    std::cout << "\nTesting URI length limit..." << std::endl;

    // Create a very long URI
    std::string long_uri = "/";
    for (int i = 0; i < 2048; ++i) {
        long_uri += "a";
    }

    const std::string raw_request =
        "GET " + long_uri + " HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::URI_TOO_LONG);

    std::cout << "  PASS" << std::endl;
}

void test_headers_too_large() {
    std::cout << "\nTesting headers size limit..." << std::endl;

    // Create a header with large value
    std::string large_header_value;
    for (int i = 0; i < 9000; ++i) {
        large_header_value += "x";
    }

    const std::string raw_request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "X-Large-Header: " + large_header_value + "\r\n"
        "\r\n";

    HttpRequest request;
    int status = request.parse(raw_request);

    assert(status == RequestStatus::HEADERS_TOO_LARGE);

    std::cout << "  PASS" << std::endl;
}

int main() {
    try {
        test_valid_simple_request();
        test_invalid_request_line();
        test_post_request_with_body();
        test_malformed_headers();
        test_missing_host_header();
        test_uri_too_long();
        test_headers_too_large();

        std::cout << "\nAll tests passed!" << std::endl;
        return 0;
    }
    catch(const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}

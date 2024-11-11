// Required methods GET, POST, DELETE

/* ===== EXAMPLE REQUEST ===== (after "svg+xml" there is a parameter: asterisk then frontslash and asterisk)
	GET / HTTP/1.1
	Host: localhost:8080
	User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml, ;q=0.8
*/

#include <iostream>
# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <set>

enum requests_types
{
	REQUEST_GET,
	REQUEST_POST,
	REQUEST_DELETE
};

struct Request
{
	std::string	method;
	std::string	path;
	std::string	http_version;
	std::map<std::string, std::string> headers;
	std::string	body;
};

bool parse_first_request_line(const std::string &request_line, Request &request)
{
	std::istringstream	iss(request_line);
	return (iss >> request.method >> request.path >> request.http_version);
}

void parse_headers(const std::string &header_line, Request &request)
{
	std::istringstream iss(header_line);
	std::string key, value;

	if (std::getline(iss, key, ':'))
	{
		std::getline(iss, value);
		// Remove any leading/trailing whitespace from the value
		if (!value.empty() && value[0] == ' ')
			value.erase(0, 1);
		request.headers[key] = value;
	}
}

void	printRequest(const Request &rq)
{
	std::cout
		<< "First line: " << std::endl
		<< rq.method << " "
		<< rq.path << " "
		<< rq.http_version << " " << std::endl
		<< "Headers: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = rq.headers.begin(); it != rq.headers.end(); it++)
			std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "Body: " << std::endl
		<< rq.body << std::endl;
}

Request	parseRequest(const std::string &raw_request)
{
	std::istringstream	request_stream(raw_request);
	Request				request;
	std::string			line;

	if (std::getline(request_stream, line) && !line.empty())
		if (!parse_first_request_line(line, request))
			throw (std::invalid_argument("Error parsing request's first line."));
	while (std::getline(request_stream, line) && !(line.size() == 1 && line[0] == '\r')) // this should be improved
		parse_headers(line, request);
	std::map<std::string, std::string>::const_iterator	it = request.headers.find("Content-Length");
	if (it != request.headers.end())
	{
		// Protect overflow through content-length being bigger than message
		int	content_length = std::stoi(it->second);
		std::string	body(content_length, '\0');
		request_stream.read(&body[0], content_length);
		request.body = body;
	}

	return (request);
}

int	main ()
{
	std::string get_request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

	std::string	post_request =
		"POST /submit HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Content-Length: 18\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "\r\n"
        "name=John\n"
		"id=1423";

	Request rq;
	std::cout << "=== Printing get request =====" << std::endl;
	rq = parseRequest(get_request);
	printRequest(rq);
	std::cout << "=== Printing post request =====" << std::endl;
	rq = parseRequest(post_request);
	printRequest(rq);
}
// Required methods GET, POST, DELETE

/* ===== EXAMPLE REQUEST ===== (after "svg+xml" there is a parameter: asterisk then frontslash and asterisk)
	GET / HTTP/1.1
	Host: localhost:8080
	User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml, ;q=0.8
*/

struct request_data
{
	int				request_type;
	// some_struct	host;
	float			http_version;
	std::string		accept;
	std::string		message;

}

#include <iostream>

// size_t find_crlf(const std::string& buffer, const std::string& delimiter)
// {
// 	if (buffer.empty())
// 		return (0);
//     return (buffer.find(delimiter));
// }

void	parseRequest(char *str)
{
	std::string str_str(str);

	parseRequest(str_str);
}

void	parseRequest(const std::string &str)
{
	size_t header_end;

	header_end = str.find("\r\n\r\n");
}

int	main ()
{
	std::string sample_request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

	parseRequest(sample_request);
}
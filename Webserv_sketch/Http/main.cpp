/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:16:44 by manuel            #+#    #+#             */
/*   Updated: 2024/11/08 11:24:41 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cstring>

class Buffer
{
	public:
		Buffer(const char * const start, size_t size);


	private:
		const char* const buffer;
		size_t size;
};

int main(void)
{
	char memoryPool[1024];

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

	std::memcpy(memoryPool, get_request.c_str(), get_request.size() + 1);


}

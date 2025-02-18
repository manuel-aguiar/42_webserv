#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void lineTests(int &testNumber);
void headerTests(int &testNumber);
void bodyTests(int &testNumber);

/**
 * 	Http Request example
 *
 *	Line 	->	POST /api/v1/users HTTP/1.1
 *	Headers	->	Host: example.com
 *				Authorization: Bearer ACCESS_TOKEN
 *				Content-Type: application/json
 *				Accept: application/json
 *				User-Agent: MyApp/1.0
 *
 *	Body	->	{
 *					"name": "John Doe",
 *					"email": "johndoe@example.com",
 *					"password": "securepassword"
 *				}
 */


int main()
{
	int	testNumber = 0;

	lineTests(testNumber);
	headerTests(testNumber);
	bodyTests(testNumber);
}

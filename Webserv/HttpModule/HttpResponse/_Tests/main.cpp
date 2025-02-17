#include "../../HttpRequest/HttpRequest.hpp"
#include "../Response.hpp"
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

void responseTests(int &testNumber);

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

	responseTests(testNumber);
}



# include "HttpResponse.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

BufferView getStatusMessage(Http::Status::Number statusCode);

/*
	Data oriented design, separate codes (needed in binary search) from messages.
	No need to load message pointers into cache when doing binary search.

	Once we found our target, we do move to the message array
*/

static int			codes	[Http::Status::_TOTAL_CODES];
static BufferView	messages[Http::Status::_TOTAL_CODES];

static int initializeCodes();
static int dummy = initializeCodes();

static int initializeCodes()
{
	size_t i = 0;

	codes[i] = Http::Status::CONTINUE;							messages[i] = BufferView("Continue");							i++;
	codes[i] = Http::Status::OK; 								messages[i] = BufferView("OK"); 								i++;
	codes[i] = Http::Status::CREATED; 							messages[i] = BufferView("Created"); 							i++;
	codes[i] = Http::Status::ACCEPTED; 							messages[i] = BufferView("Accepted"); 							i++;
	codes[i] = Http::Status::NO_CONTENT; 						messages[i] = BufferView("No Content"); 						i++;
	codes[i] = Http::Status::RESET_CONTENT; 					messages[i] = BufferView("Reset Content");						i++;
	codes[i] = Http::Status::PARTIAL_CONTENT; 					messages[i] = BufferView("Partial Content"); 					i++;
	// 3xx Redirection
	codes[i] = Http::Status::MULTIPLE_CHOICES; 					messages[i] = BufferView("Multiple Choices"); 					i++;
	codes[i] = Http::Status::MOVED_PERMANENTLY; 				messages[i] = BufferView("Moved Permanently");					i++;
	codes[i] = Http::Status::FOUND; 							messages[i] = BufferView("Found"); 								i++;
	codes[i] = Http::Status::SEE_OTHER; 						messages[i] = BufferView("See Other"); 							i++;
	codes[i] = Http::Status::NOT_MODIFIED; 						messages[i] = BufferView("Not Modified"); 						i++;
	codes[i] = Http::Status::TEMPORARY_REDIRECT; 				messages[i] = BufferView("Temporary Redirect"); 				i++;
	codes[i] = Http::Status::PERMANENT_REDIRECT; 				messages[i] = BufferView("Permanent Redirect"); 				i++;
	// 4xx Client Errors
	codes[i] = Http::Status::BAD_REQUEST; 						messages[i] = BufferView("Bad Request"); 						i++;
	codes[i] = Http::Status::FORBIDDEN; 						messages[i] = BufferView("Forbidden"); 							i++;
	codes[i] = Http::Status::NOT_FOUND; 						messages[i] = BufferView("Not Found"); 							i++;
	codes[i] = Http::Status::METHOD_NOT_ALLOWED; 				messages[i] = BufferView("Method Not Allowed"); 				i++;
	codes[i] = Http::Status::NOT_ACCEPTABLE; 					messages[i] = BufferView("Not Acceptable"); 					i++;
	codes[i] = Http::Status::LENGTH_REQUIRED; 					messages[i] = BufferView("Length Required"); 					i++;
	codes[i] = Http::Status::PRECONDITION_FAILED; 				messages[i] = BufferView("Precondition Failed"); 				i++;
	codes[i] = Http::Status::PAYLOAD_TOO_LARGE; 				messages[i] = BufferView("Payload Too Large"); 					i++;
	codes[i] = Http::Status::URI_TOO_LONG; 						messages[i] = BufferView("URI Too Long"); 						i++;
	codes[i] = Http::Status::UNSUPPORTED_MEDIA_TYPE; 			messages[i] = BufferView("Unsupported Media Type"); 			i++;
	codes[i] = Http::Status::EXPECTATION_FAIL;					messages[i] = BufferView("Expectation Fail");					i++;
	codes[i] = Http::Status::TOO_MANY_REQUESTS; 				messages[i] = BufferView("Too Many Requests"); 					i++;
	codes[i] = Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE; 	messages[i] = BufferView("Request Header Fields Too Large");	i++;
	codes[i] = Http::Status::UNAVAILABLE_FOR_LEGAL_REASONS; 	messages[i] = BufferView("Unavailable For Legal Reasons"); 		i++;
	// 5xx Server Errors
	codes[i] = Http::Status::INTERNAL_ERROR; 					messages[i] = BufferView("Internal Server Error"); 				i++;
	codes[i] = Http::Status::NOT_IMPLEMENTED; 					messages[i] = BufferView("Not Implemented");					i++;
	codes[i] = Http::Status::BAD_GATEWAY; 						messages[i] = BufferView("Bad Gateway"); 						i++;
	codes[i] = Http::Status::SERVICE_UNAVAILABLE; 				messages[i] = BufferView("Service Unavailable"); 				i++;
	codes[i] = Http::Status::GATEWAY_TIMEOUT; 					messages[i] = BufferView("Gateway Timeout"); 					i++;
	codes[i] = Http::Status::HTTP_VERSION_NOT_SUPPORTED; 		messages[i] = BufferView("HTTP Version Not Supported"); 		i++;
	codes[i] = Http::Status::VARIANT_ALSO_NEGOTIATES; 			messages[i] = BufferView("Variant Also Negotiates"); 			i++;
	codes[i] = Http::Status::INSUFFICIENT_STORAGE; 				messages[i] = BufferView("Insufficient Storage"); 				i++;
	codes[i] = Http::Status::LOOP_DETECTED; 					messages[i] = BufferView("Loop Detected"); 						i++;
	codes[i] = Http::Status::NOT_EXTENDED; 						messages[i] = BufferView("Not Extended"); 						i++;
	codes[i] = Http::Status::NETWORK_AUTHENTICATION_REQUIRED; 	messages[i] = BufferView("Network Authentication Required"); 	i++;

	// size test
	ASSERT_EQUAL(i, Http::Status::_TOTAL_CODES, "Status code array size mismatch");

	// making sure the array is written as sorted, no duplicates and strictly ascending status code order
	#ifndef NDEBUG
		for (size_t i = 1; i < sizeof(codes) / sizeof(codes[0]) - 1; i++)
			ASSERT_EQUAL(codes[i] > codes[i - 1], true, "Status codes are not sorted/unique");
	#endif

	return (1);
}


BufferView getStatusMessage(Http::Status::Number statusCode)
{
	// binary search in the code array
	size_t low = 0;
	size_t high = (sizeof(codes)/ sizeof(codes[0])) - 1;
	size_t mid;

	while (low < high)
	{
		mid = low + ((high - low) / 2);
		if (statusCode == codes[mid])
			return (messages[mid]);
		else if (statusCode < codes[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}
	// assert the statusCode found actually is in the array
	ASSERT_EQUAL(statusCode == codes[low], true, "Status code not found in message map");

	// all good, return
	return (messages[low]);
}

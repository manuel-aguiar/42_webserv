

# include "HttpResponse.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

const char* getStatusMessage(int statusCode);

/*
	Data oriented design, separate codes (needed in binary search) from messages.
	No need to load message pointers into cache when doing binary search.

	Once we found our target, we do move to the message array
*/
const char* getStatusMessage(int statusCode)
{
	static int			codes	[Http::Status::_TOTAL_CODES];
	static const char*	messages[Http::Status::_TOTAL_CODES];
	static bool 		initialized;

	if (!initialized)
	{
		size_t i = 0;

		codes[i] = Http::Status::OK; 								messages[i] = "OK"; 								i++;
		codes[i] = Http::Status::CREATED; 							messages[i] = "Created"; 							i++;
		codes[i] = Http::Status::ACCEPTED; 							messages[i] = "Accepted"; 							i++;
		codes[i] = Http::Status::NO_CONTENT; 						messages[i] = "No Content"; 						i++;
		codes[i] = Http::Status::RESET_CONTENT; 					messages[i] = "Reset Content";						i++;
		codes[i] = Http::Status::PARTIAL_CONTENT; 					messages[i] = "Partial Content"; 					i++;
		// 3xx Redirection
		codes[i] = Http::Status::MULTIPLE_CHOICES; 					messages[i] = "Multiple Choices"; 					i++;
		codes[i] = Http::Status::MOVED_PERMANENTLY; 				messages[i] = "Moved Permanently";					i++;
		codes[i] = Http::Status::FOUND; 							messages[i] = "Found"; 								i++;
		codes[i] = Http::Status::SEE_OTHER; 						messages[i] = "See Other"; 							i++;
		codes[i] = Http::Status::NOT_MODIFIED; 						messages[i] = "Not Modified"; 						 i++;
		codes[i] = Http::Status::TEMPORARY_REDIRECT; 				messages[i] = "Temporary Redirect"; 				i++;
		codes[i] = Http::Status::PERMANENT_REDIRECT; 				messages[i] = "Permanent Redirect"; 				i++;
		// 4xx Client Errors
		codes[i] = Http::Status::BAD_REQUEST; 						messages[i] = "Bad Request"; 						i++;
		codes[i] = Http::Status::FORBIDDEN; 						messages[i] = "Forbidden"; 							i++;
		codes[i] = Http::Status::NOT_FOUND; 						messages[i] = "Not Found"; 							i++;
		codes[i] = Http::Status::METHOD_NOT_ALLOWED; 				messages[i] = "Method Not Allowed"; 				i++;
		codes[i] = Http::Status::NOT_ACCEPTABLE; 					messages[i] = "Not Acceptable"; 					i++;
		codes[i] = Http::Status::LENGTH_REQUIRED; 					messages[i] = "Length Required"; 					i++;
		codes[i] = Http::Status::PRECONDITION_FAILED; 				messages[i] = "Precondition Failed"; 				i++;
		codes[i] = Http::Status::PAYLOAD_TOO_LARGE; 				messages[i] = "Payload Too Large"; 					i++;
		codes[i] = Http::Status::URI_TOO_LONG; 						messages[i] = "URI Too Long"; 						i++;
		codes[i] = Http::Status::UNSUPPORTED_MEDIA_TYPE; 			messages[i] = "Unsupported Media Type"; 			i++;
		codes[i] = Http::Status::TOO_MANY_REQUESTS; 				messages[i] = "Too Many Requests"; 					i++;
		codes[i] = Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE; 	messages[i] = "Request Header Fields Too Large";	i++;
		codes[i] = Http::Status::UNAVAILABLE_FOR_LEGAL_REASONS; 	messages[i] = "Unavailable For Legal Reasons"; 		i++;

		// 5xx Server Errors
		codes[i] = Http::Status::INTERNAL_ERROR; 					messages[i] = "Internal Server Error"; 				i++;
		codes[i] = Http::Status::NOT_IMPLEMENTED; 					messages[i] = "Not Implemented";					i++;
		codes[i] = Http::Status::BAD_GATEWAY; 						messages[i] = "Bad Gateway"; 						i++;
		codes[i] = Http::Status::SERVICE_UNAVAILABLE; 				messages[i] = "Service Unavailable"; 				i++;
		codes[i] = Http::Status::GATEWAY_TIMEOUT; 					messages[i] = "Gateway Timeout"; 					i++;
		codes[i] = Http::Status::HTTP_VERSION_NOT_SUPPORTED; 		messages[i] = "HTTP Version Not Supported"; 		i++;
		codes[i] = Http::Status::VARIANT_ALSO_NEGOTIATES; 			messages[i] = "Variant Also Negotiates"; 			i++;
		codes[i] = Http::Status::INSUFFICIENT_STORAGE; 				messages[i] = "Insufficient Storage"; 				i++;
		codes[i] = Http::Status::LOOP_DETECTED; 					messages[i] = "Loop Detected"; 						i++;
		codes[i] = Http::Status::NOT_EXTENDED; 						messages[i] = "Not Extended"; 						i++;
		codes[i] = Http::Status::NETWORK_AUTHENTICATION_REQUIRED; 	messages[i] = "Network Authentication Required"; 	i++;

		initialized = true;

		// size test
		ASSERT_EQUAL(i, Http::Status::_TOTAL_CODES, "Status code array size mismatch");

		// making sure the array is written as sorted, no duplicates and strictly ascending status code order
		#ifndef NDEBUG
			for (size_t i = 1; i < sizeof(codes) / sizeof(codes[0]) - 1; i++)
				ASSERT_EQUAL(codes[i] > codes[i - 1], true, "Status codes are not sorted/unique");
		#endif
	}

	// binary search in the code array
	size_t low = 0;
	size_t high = (sizeof(codes)/ sizeof(codes[0])) - 1;
	size_t mid;

	while (low <= high)
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

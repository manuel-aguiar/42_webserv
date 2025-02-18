
# include "../HttpDefinitions.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

struct HttpCode
{
	int			code;
	const char*	message;
};

const char* getStatusMessage(int statusCode)
{
	static const HttpCode codeMessage[] =
	{
		{Http::Status::OK, "OK"},
		{Http::Status::CREATED, "Created"},
		{Http::Status::ACCEPTED, "Accepted"},
		{Http::Status::NO_CONTENT, "No Content"},
		{Http::Status::RESET_CONTENT, "Reset Content"},
		{Http::Status::PARTIAL_CONTENT, "Partial Content"},
		// 3xx Redirection
		{Http::Status::MULTIPLE_CHOICES, "Multiple Choices"},
		{Http::Status::MOVED_PERMANENTLY, "Moved Permanently"},
		{Http::Status::FOUND, "Found"},
		{Http::Status::SEE_OTHER, "See Other"},
		{Http::Status::NOT_MODIFIED, "Not Modified"},
		{Http::Status::TEMPORARY_REDIRECT, "Temporary Redirect"},
		{Http::Status::PERMANENT_REDIRECT, "Permanent Redirect"},
		// 4xx Client Errors
		{Http::Status::BAD_REQUEST, "Bad Request"},
		{Http::Status::FORBIDDEN, "Forbidden"},
		{Http::Status::NOT_FOUND, "Not Found"},
		{Http::Status::METHOD_NOT_ALLOWED, "Method Not Allowed"},
		{Http::Status::PRECONDITION_FAILED, "Precondition Failed"},
		{Http::Status::PAYLOAD_TOO_LARGE, "Payload Too Large"},
		{Http::Status::URI_TOO_LONG, "URI Too Long"},
		{Http::Status::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
		{Http::Status::TOO_MANY_REQUESTS, "Too Many Requests"},
		{Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"},
		{Http::Status::UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons"},
		// 5xx Server Errors
		{Http::Status::INTERNAL_ERROR, "Internal Server Error"},
		{Http::Status::NOT_IMPLEMENTED, "Not Implemented"},
		{Http::Status::BAD_GATEWAY, "Bad Gateway"},
		{Http::Status::SERVICE_UNAVAILABLE, "Service Unavailable"},
		{Http::Status::GATEWAY_TIMEOUT, "Gateway Timeout"},
		{Http::Status::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"},
		{Http::Status::VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"},
		{Http::Status::INSUFFICIENT_STORAGE, "Insufficient Storage"},
		{Http::Status::LOOP_DETECTED, "Loop Detected"},
		{Http::Status::NOT_EXTENDED, "Not Extended"},
		{Http::Status::NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"}	
	};

	static int			codes[sizeof(codeMessage) / sizeof(HttpCode)]; 
	static const char*	messages[sizeof(codeMessage) / sizeof(HttpCode)];
	static bool 		initialized;

	if (!initialized)
	{
		// making sure the array is written as sorted, no duplicates and strictly ascending status code order
		#ifndef NDEBUG
			for (size_t i = 1; i < sizeof(codeMessage) / sizeof(HttpCode); i++)
				ASSERT_EQUAL(codeMessage[i].code > codeMessage[i - 1].code, true, "Status codes are not sorted/unique");
		#endif
		
		// assigning values the first time
		for (size_t i = 0; i < sizeof(codeMessage) / sizeof(HttpCode); i++)
		{
			codes[i] = codeMessage[i].code;
			messages[i] = codeMessage[i].message;
		}
		initialized = true;
	}

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
	ASSERT_EQUAL(statusCode == codes[low], true, "Status code not found in message map");

	return (messages[low]);
}
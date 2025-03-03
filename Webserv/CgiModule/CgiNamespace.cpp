

# include "CgiNamespace.h"

namespace Cgi
{
    Header::Header(const char* key, const char* value) : key(key), value(value) {}
    Header::Header(const BufferView& key, const BufferView& value) : key(key), value(value) {}

}

#ifndef NDEBUG

	static int testHeadersOfInterest();
	static const int g_testHeadersOfInterest = testHeadersOfInterest();
	static int testHeadersOfInterest()
	{
		std::string test = Cgi::RequestConsts::Header::Status;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() 
			== BufferView(Cgi::RequestConsts::Header::Status), true, "Status is not correctly formated");
		test =  Cgi::RequestConsts::Header::ContentType;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() 
			== BufferView( Cgi::RequestConsts::Header::ContentType), true, " ContentType is not correctly formated");	
		test = Cgi::RequestConsts::Header::Location;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() 
			== BufferView(Cgi::RequestConsts::Header::Location), true, "Location is not correctly formated");
		return (0);
	}
#endif

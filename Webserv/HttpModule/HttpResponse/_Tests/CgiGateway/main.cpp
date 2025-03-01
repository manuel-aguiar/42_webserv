
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../HttpCgiGateway/HttpCgiGateway.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../Globals/Globals.hpp"
#include "../../../../Events/Manager/Manager.hpp"
#include "../../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void test1(int& testNumber)
{
	TEST_INTRO(testNumber++);

	ServerContext 			context;
	Globals 				globals(NULL, NULL, NULL, NULL);
	Events::Manager 		eventManager(30, globals);
	Cgi::Module 			cgi(10, 100, 5000, eventManager, globals);

	context.setAddonLayer(Ws::AddonLayer::CGI, &cgi);
	
	try
	{
		Http::Response 		response(context);
		Http::RequestData 	requestData;
		Http::ResponseData 	responseData;


		TEST_PASSED_MSG("Cgi Tests");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}



int main(void)
{

	TEST_HEADER("Cgi Gateway ");

	int testNumber = 1;

	test1(testNumber);

	TEST_FOOTER;

	return (0);
}
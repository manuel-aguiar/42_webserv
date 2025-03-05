#ifndef MOCK_DEPENDENCIES_HPP
#define MOCK_DEPENDENCIES_HPP

# include "../../../HttpRequest/HttpRequest.hpp"
// just for testing
namespace Http
{
	// implementation just to satisfy the linker


	// Additional stubs for testing
	// bool Response::mf_validateHeaders()
	// {
	// 	return true; // Skip validation for tests
	// }

	// For avoiding connection address errors
	// Http::IOStatus::Type Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	// {
	// 	(void)writeBuffer;
	// 	return Http::IOStatus::FINISHED;
	// }
}

#endif
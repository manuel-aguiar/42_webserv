#ifndef MOCK_DEPENDENCIES_HPP
#define MOCK_DEPENDENCIES_HPP

// just for testing
namespace Http
{
	// implementation just to satisfy the linker
	void Response::mf_assembleTargetPath()
	{
		// Do nothing - this is just a for testing
	}

	// Additional stubs for testing
	// bool Response::mf_validateHeaders()
	// {
	// 	return true; // Skip validation for tests
	// }

	// For avoiding connection address errors
	// Http::ResponseStatus::Type Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	// {
	// 	(void)writeBuffer;
	// 	return Http::ResponseStatus::FINISHED;
	// }
}

#endif
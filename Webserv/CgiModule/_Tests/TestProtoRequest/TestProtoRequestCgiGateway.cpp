

# include "TestProtoRequest.hpp"

void
(*TestProtoRequest_CgiGateway::Callbacks[CgiModule::CALLBACK_COUNT])(CgiModule::User user) = {
	TestProtoRequest_CgiGateway::onSuccess,
	TestProtoRequest_CgiGateway::onErrorStartup,
	TestProtoRequest_CgiGateway::onErrorRuntime,
	TestProtoRequest_CgiGateway::onErrorTimeOut,
};

void
TestProtoRequest_CgiGateway::onSuccess(CgiModule::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onErrorStartup(CgiModule::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->falseStartCgi();
}

void
TestProtoRequest_CgiGateway::onErrorRuntime(CgiModule::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->cancelCgi();
}

void
TestProtoRequest_CgiGateway::onErrorTimeOut(CgiModule::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->timeoutCgi();
}


CgiModule::BytesRead
TestProtoRequest_CgiGateway::onWrite(CgiModule::User user, int writeFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiWrite(writeFd));
}

CgiModule::BytesWritten
TestProtoRequest_CgiGateway::onRead(CgiModule::User user, int readFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiRead(readFd));
}


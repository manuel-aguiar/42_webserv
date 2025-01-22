

# include "TestProtoRequest.hpp"

void
(*TestProtoRequest_CgiGateway::Callbacks[CgiNotify::COUNT])(CgiUser user) = {
	TestProtoRequest_CgiGateway::onSuccess,
	TestProtoRequest_CgiGateway::onErrorStartup,
	TestProtoRequest_CgiGateway::onErrorRuntime,
	TestProtoRequest_CgiGateway::onErrorTimeOut,
};

void
TestProtoRequest_CgiGateway::onSuccess(CgiUser user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onErrorStartup(CgiUser user)
{
	reinterpret_cast<TestProtoRequest*>(user)->falseStartCgi();
}

void
TestProtoRequest_CgiGateway::onErrorRuntime(CgiUser user)
{
	reinterpret_cast<TestProtoRequest*>(user)->cancelCgi();
}

void
TestProtoRequest_CgiGateway::onErrorTimeOut(CgiUser user)
{
	reinterpret_cast<TestProtoRequest*>(user)->timeoutCgi();
}


CgiIO::BytesCount
TestProtoRequest_CgiGateway::onWrite(CgiUser user, int writeFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiWrite(writeFd));
}

CgiIO::BytesCount
TestProtoRequest_CgiGateway::onRead(CgiUser user, int readFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiRead(readFd));
}


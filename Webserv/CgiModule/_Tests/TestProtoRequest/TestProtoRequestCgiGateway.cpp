

# include "TestProtoRequest.hpp"

void
(*TestProtoRequest_CgiGateway::Callbacks[Cgi::Notify::COUNT])(Cgi::User user) = {
	TestProtoRequest_CgiGateway::onSuccess,
	TestProtoRequest_CgiGateway::onErrorStartup,
	TestProtoRequest_CgiGateway::onErrorRuntime,
	TestProtoRequest_CgiGateway::onErrorTimeOut,
};

void
TestProtoRequest_CgiGateway::onSuccess(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onErrorStartup(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->falseStartCgi();
}

void
TestProtoRequest_CgiGateway::onErrorRuntime(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->cancelCgi();
}

void
TestProtoRequest_CgiGateway::onErrorTimeOut(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->timeoutCgi();
}


Cgi::IO::BytesCount
TestProtoRequest_CgiGateway::onWrite(Cgi::User user, int writeFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiWrite(writeFd));
}

Cgi::IO::BytesCount
TestProtoRequest_CgiGateway::onRead(Cgi::User user, int readFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiRead(readFd));
}


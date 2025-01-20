

# include "TestProtoRequest.hpp"

void
(*TestProtoRequest_CgiGateway::Callbacks[Cgi::Module::Runtime_Callback::COUNT])(Cgi::Module::User user) = {
	TestProtoRequest_CgiGateway::onSuccess,
	TestProtoRequest_CgiGateway::onErrorStartup,
	TestProtoRequest_CgiGateway::onErrorRuntime,
	TestProtoRequest_CgiGateway::onErrorTimeOut,
};

void
TestProtoRequest_CgiGateway::onSuccess(Cgi::Module::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onErrorStartup(Cgi::Module::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->falseStartCgi();
}

void
TestProtoRequest_CgiGateway::onErrorRuntime(Cgi::Module::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->cancelCgi();
}

void
TestProtoRequest_CgiGateway::onErrorTimeOut(Cgi::Module::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->timeoutCgi();
}


Cgi::Module::IO_Callback::BytesCount
TestProtoRequest_CgiGateway::onWrite(Cgi::Module::User user, int writeFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiWrite(writeFd));
}

Cgi::Module::IO_Callback::BytesCount
TestProtoRequest_CgiGateway::onRead(Cgi::Module::User user, int readFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiRead(readFd));
}


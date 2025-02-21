

# include "TestProtoRequest.hpp"

void
TestProtoRequest_CgiGateway::onSuccess(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onError(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->FailureCgi();
}

Cgi::IO::State
TestProtoRequest_CgiGateway::onWrite(Cgi::User user, const Ws::fd writeFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiWrite(writeFd));
}

Cgi::IO::State
TestProtoRequest_CgiGateway::onRead(Cgi::User user, const Ws::fd readFd)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiRead(readFd));
}

Cgi::IO::State		
TestProtoRequest_CgiGateway::onReceiveHeaders(Cgi::User user, Cgi::HeaderData& headers)
{
	return (reinterpret_cast<TestProtoRequest*>(user)->CgiReceiveHeaders(headers));
}



# include "TestProtoRequest.hpp"

void
TestProtoRequest_CgiGateway::onSuccess(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->SuccessCgi();
}

void
TestProtoRequest_CgiGateway::onError(Cgi::User user)
{
	reinterpret_cast<TestProtoRequest*>(user)->cancelCgi();
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


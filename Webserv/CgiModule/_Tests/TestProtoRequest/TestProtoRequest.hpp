

#ifndef TESTPROTOREQUEST_HPP

# define TESTPROTOREQUEST_HPP

#include "../../CgiNamespace.h"
#include "../../../CgiModule/Request/Request.hpp"
#include "../../../CgiModule/Module/Module.hpp"
#include "../../../GenericUtils/Buffer/Buffer.hpp"
class Globals;

class TestProtoRequest
{
    public:
		TestProtoRequest(Globals& globals, Cgi::Module& cgi, int id);
		~TestProtoRequest();
		TestProtoRequest(const TestProtoRequest& copy);
		

		// Callbacks for the CGI Module
		void   	SuccessCgi();		//On execute
		void	falseStartCgi();	// on error startup
		void	FailureCgi();		// on error runtime
		void	timeoutCgi();

		Cgi::IO::State		CgiRead(int readFd);
		Cgi::IO::State		CgiWrite(int writeFd);
		Cgi::IO::State		CgiReceiveHeaders(const Cgi::HeaderData& headers);

		void	debugPrint() const;

		enum e_CgiResultStatus
		{
			E_CGI_STATUS_WORKING,
			E_CGI_STATUS_FAILED_ACQUIRE,
			E_CGI_STATUS_SUCCESS,
			E_CGI_STATUS_ERROR,
			E_CGI_STATUS_COUNT,
		};

		void	printBufStdout();

		Globals&			m_globals;
		Cgi::Module&		m_cgi;
		
		Cgi::Request*		m_CgiRequestData;

		std::string			m_msgBody;
		
		Buffer<40960>		m_buffer;
		size_t				m_TotalBytesRead;

		int					m_id;

		int					m_headerStatus;

		std::string			m_ExpectedOutput;

		e_CgiResultStatus	m_CgiResultStatus;
};

class TestProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onSuccess(Cgi::User user);
		static void onError(Cgi::User user);

		static Cgi::IO::State 		onRead(Cgi::User user, const Ws::fd readFd);
		static Cgi::IO::State 		onWrite(Cgi::User user, const Ws::fd writeFd);
		static Cgi::IO::State		onReceiveHeaders(Cgi::User user, const Cgi::HeaderData& headers);
};




#endif
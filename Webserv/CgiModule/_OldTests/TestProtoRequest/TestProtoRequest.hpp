

#ifndef TESTPROTOREQUEST_HPP

# define TESTPROTOREQUEST_HPP

#include "../../../CgiModule/Request/Request.hpp"
#include "../../../CgiModule/Module/Module.hpp"

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

		Cgi::IO::BytesCount		CgiRead(int readFd);
		Cgi::IO::BytesCount		CgiWrite(int writeFd);

		void	debugPrint() const;

		enum e_CgiResultStatus
		{
			E_CGI_STATUS_WORKING,
			E_CGI_STATUS_FAILED_ACQUIRE,
			E_CGI_STATUS_SUCCESS,
			E_CGI_STATUS_ERROR_STARTUP,
			E_CGI_STATUS_ERROR,
			E_CGI_STATUS_TIMEOUT,
			E_CGI_STATUS_COUNT,
		};

		void	printBufStdout();

		Globals&			m_globals;
		Cgi::Module&		m_cgi;
		
		Cgi::Request*		m_CgiRequestData;

		std::string			m_msgBody;
		
		char 				m_buffer[1024];
		size_t				m_TotalBytesRead;

		int					m_id;

		std::string			m_ExpectedOutput;

		e_CgiResultStatus	m_CgiResultStatus;
};

class TestProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onSuccess(Cgi::User user);
		static void onError(Cgi::User user);

		static Cgi::IO::BytesCount 		onRead(Cgi::User user, const Ws::fd readFd);
		static Cgi::IO::BytesCount 		onWrite(Cgi::User user, const Ws::fd writeFd);
};




#endif


#ifndef TESTPROTOREQUEST_HPP

# define TESTPROTOREQUEST_HPP

#include "../../../Callback/Callback.hpp"
#include "../../CgiRequest/CgiRequest.hpp"
#include "../../CgiModule/CgiModule.hpp"

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
		void	cancelCgi();		// on error runtime
		void	timeoutCgi();

		Cgi::Module::BytesRead		CgiRead(int readFd);
		Cgi::Module::BytesWritten		CgiWrite(int writeFd);

		void	debugPrint() const;

		enum e_CgiResultStatus
		{
			E_CGI_STATUS_WORKING,
			E_CGI_STATUS_FAILED_ACQUIRE,
			E_CGI_STATUS_SUCCESS,
			E_CGI_STATUS_ERROR_STARTUP,
			E_CGI_STATUS_ERROR_RUNTIME,
			E_CGI_STATUS_TIMEOUT,
			E_CGI_STATUS_COUNT,
		};

		void	printBufStdout();

		Globals&			m_globals;
		Cgi::Module&			m_cgi;
		
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
		static void onSuccess(Cgi::Module::User user);
		static void onErrorStartup(Cgi::Module::User user);
		static void onErrorRuntime(Cgi::Module::User user);
		static void onErrorTimeOut(Cgi::Module::User user);

		static Cgi::Module::BytesRead 		onRead(Cgi::Module::User user, int readFd);
		static Cgi::Module::BytesWritten 		onWrite(Cgi::Module::User user, int writeFd);

		static void (*Callbacks[Cgi::Module::CALLBACK_COUNT])(Cgi::Module::User user);
};




#endif
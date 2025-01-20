

// Project headers
# include "../../CgiModule/CgiModule.hpp"

// test helpers
# include "../TestProtoRequest/TestProtoRequest.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../../Toolkit/_Tests/test.h"
# include "CgiStressTest.hpp"

//C++ headers
# include <iostream>
# include <iomanip>

// C headers
#include <fstream>
#include <unistd.h>

extern void prepareExpectedOutput(bool isExpectedValid, TestProtoRequest& proto);

extern std::vector<std::string> g_mockGlobals_ErrorMsgs;


std::string	CgiStressTest::ValgrindReadandClear(const char* filename)
{
    std::ifstream logFileRead(filename);

	// if the file doesn't exist, means we are not using valgrind and directing output to a file,
	//move on
	std::string result;
    if (!logFileRead.is_open())
		return (result);

    // Skip some weird non printable characters at the beginning
    char c;
    while (logFileRead.get(c)) {
        if (std::isprint(static_cast<unsigned char>(c))) {
            logFileRead.putback(c);
            break;
        }
    }

	// actually read
	std::string line;
	while (std::getline(logFileRead, line))
		result += line + '\n';
	logFileRead.close();

	//clear content
	std::ofstream logFileClear(filename, std::ios::trunc);
	
	if (logFileClear.is_open())
	{
		logFileClear.flush();
		logFileClear.close();
	}
	return (result);
}

// Tests about the resilience of the Module under heavy load

void	CgiStressTest::ValidPython(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("py");
	proto.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

	// some env vars
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "123");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "DoubledBasic");

	prepareExpectedOutput(true, proto);
}


void	CgiStressTest::ValidShell(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("sh");
	proto.m_CgiRequestData->setScriptPath("TestScripts/sh/envPrint.sh");

	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::PATH_INFO, "LostMyWay");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY1", "hiThere");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "321");

	prepareExpectedOutput(true, proto);
}

void	CgiStressTest::ValidPHP(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("php");
	proto.m_CgiRequestData->setScriptPath("TestScripts/php/envPrint.php");

	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::PATH_INFO, "WhereAmI");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY3", "SomeHttpStuff_in_here");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "charIsUserFault");

	prepareExpectedOutput(true, proto);
}

void	CgiStressTest::InvalidInterpreter(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("pyasgasg");
	proto.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "123");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "DoubledBasic");

	prepareExpectedOutput(false, proto);
}

void	CgiStressTest::InvalidInterpreterPath(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("phpbad");
	proto.m_CgiRequestData->setScriptPath("TestScripts/php/envPrint.php");

	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "123");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "DoubledBasic");

	prepareExpectedOutput(false, proto);
}


void	CgiStressTest::InvalidScript(TestProtoRequest& proto, int index)
{
	(void)index;
	proto.m_CgiRequestData->setExtension("py");
	proto.m_CgiRequestData->setScriptPath("thisDoesNotExist");

	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "123");
	proto.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");
	proto.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "DoubledBasic");

	prepareExpectedOutput(false, proto);
}


void	CgiStressTest::MixedCriteria(TestProtoRequest& proto, int index)
{
	switch (index % 6)
	{
		case 0:
			ValidPython(proto); break;
		case 1:
			ValidShell(proto); break;
		case 2:
			ValidPHP(proto); break;
		case 3:
			InvalidScript(proto); break;
		case 4:
			InvalidInterpreter(proto); break;
		case 5:
			InvalidInterpreterPath(proto); break;
	}
}

void	CgiStressTest::AllValidCriteria(TestProtoRequest& proto, int index)
{
	switch (index % 3)
	{
		case 0:
			ValidPython(proto); break;
		case 1:
			ValidShell(proto); break;
		case 2:
			ValidPHP(proto); break;
	}
}

void	CgiStressTest::AllInvalidCriteria(TestProtoRequest& proto, int index)
{
	switch (index % 3)
	{
		case 0:
			InvalidScript(proto); break;
		case 1:
			InvalidInterpreter(proto); break;
		case 2:
			InvalidInterpreterPath(proto); break;
	}
}

int CgiStressTest::StressTest(int testNumber, 
					const int workers, 
					const int backlog, 
					const int connectionCount, 
					const unsigned int timeoutMs,
					void (*AssignmentCriteria)(TestProtoRequest& proto, int index),
					const char* assignmentDescription)
{
	try
	{
		std::cout << "TEST " << testNumber++ << ": "  << assignmentDescription << '\n';

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(workers, backlog, timeoutMs, eventManager, globals);

		// collects error messages, we can't throw cause we are redirecting stderr
		std::string FailureMessages;
		unsigned int nextWait;
		size_t statusCounter[TestProtoRequest::E_CGI_STATUS_COUNT] = {0};


		/// setting up some fds to divert interpreter's error messages for "no such file or directory"
		int testpipe[2];
		int stdcerrDup = dup(STDERR_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDERR_FILENO);
		FileDescriptor::setNonBlocking(testpipe[0]);
		char pipeDrain[1024];
		/////////////////


		cgi.addInterpreter("py", "/usr/bin/python3");
		cgi.addInterpreter("sh", "/usr/bin/bash");
		cgi.addInterpreter("php", "/usr/bin/php");
		cgi.addInterpreter("phpbad", "asgasgasgasg");

		DynArray<TestProtoRequest> requests;
		requests.reserve(connectionCount);

		for (int i = 0; i < connectionCount; ++i)
		{
			requests.emplace_back(globals, cgi, i);
			requests.back().m_CgiRequestData = cgi.acquireRequest();
			
			if (requests.back().m_CgiRequestData == NULL)
			{
				// keep processing even though this proto didn't get a connection
				requests.back().m_CgiResultStatus = TestProtoRequest::E_CGI_STATUS_FAILED_ACQUIRE;

				nextWait = cgi.processRequests();
				if (eventManager.getSubscribeCount() != 0)
					eventManager.ProcessEvents(nextWait);

				//pipedrain
				while (read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);
				continue;
			}
			requests.back().m_CgiRequestData->setTimeoutMs(timeoutMs);
			
			// setup callbacks
			requests.back().m_CgiRequestData->setUser(&requests.back());
			requests.back().m_CgiRequestData->setUserCallback(Cgi::Module::CALLBACK_ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
			requests.back().m_CgiRequestData->setUserCallback(Cgi::Module::CALLBACK_ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
			requests.back().m_CgiRequestData->setUserCallback(Cgi::Module::CALLBACK_ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
			requests.back().m_CgiRequestData->setUserCallback(Cgi::Module::CALLBACK_ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
			requests.back().m_CgiRequestData->setReadHandler(&TestProtoRequest_CgiGateway::onRead);
			requests.back().m_CgiRequestData->setWriteHandler(&TestProtoRequest_CgiGateway::onWrite);

			AssignmentCriteria(requests.back(), i);
			
			cgi.enqueueRequest(*requests.back().m_CgiRequestData, false);

			// process events right now at each loop, that way we make room in the Module
			// to take more clients
			nextWait = cgi.processRequests();
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);

			
			//pipedrain not to sigpipe the failed interpreters
			while (read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);

		}


		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
			while (::read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);
		}

		if (eventManager.getSubscribeCount() != 0)
			FailureMessages = FailureMessages + "EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);	

		if (cgi.getBusyWorkerCount() != 0)
			FailureMessages = FailureMessages + "Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);


		// checking results
		for (int i = 0; i < connectionCount; ++i)
		{
			statusCounter[requests[i].m_CgiResultStatus]++;
			switch (requests[i].m_CgiResultStatus)
			{
				case TestProtoRequest::E_CGI_STATUS_WORKING:
					FailureMessages = FailureMessages + StringUtils::to_string(i) + " still working" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);
					break;
				case TestProtoRequest::E_CGI_STATUS_SUCCESS:

					if (::strlen(requests[i].m_buffer) != requests[i].m_ExpectedOutput.size()
					|| std::string(requests[i].m_buffer) != requests[i].m_ExpectedOutput)
					{
						FailureMessages = FailureMessages + '\n' + StringUtils::to_string(i) + " failed: " 
						+ StringUtils::to_string(requests[i].m_TotalBytesRead) + " got:\n" + requests[i].m_buffer + "\n\n"
						+ "expected:\n" + StringUtils::to_string(requests[i].m_ExpectedOutput.length()) + " " + requests[i].m_ExpectedOutput + "\n\n";
					}
					break ;
				default:
					break ;
			}
		}


		// restoring the original stdcerr not to mess the remaining tests
		dup2(testpipe[1], stdcerrDup);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]);
		/////////////////////////////


		std::string valLog = CgiStressTest::ValgrindReadandClear("valgrind_output.log");
		if (!valLog.empty())
			FailureMessages = FailureMessages + "Valgrind errors in this test:\n\n" + valLog + "\n\n" 
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

			
		std::cout << "\t=================================" << std::endl
		<< "\t| success:           " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_SUCCESS] << " |\n"
		<< "\t| failed acquire:    " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_FAILED_ACQUIRE] << " |\n"
		<< "\t| timedout:          " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_TIMEOUT] << " |\n"
		<< "\t| error startup:     " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_ERROR_STARTUP] << " |\n"
		<< "\t| error runtime:     " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_ERROR_RUNTIME] << " |\n"
		<< "\t| working:           " << std::setw(10) << statusCounter[TestProtoRequest::E_CGI_STATUS_WORKING] << " |\n"
		<< "\t---------------------------------" << '\n'
		<< "\t| Total Connections: " << std::setw(10) << connectionCount << " |\n"
		<< "\t| Total Workers:     " << std::setw(10) << workers << " |\n"
		<< "\t| Total Backlog:     " << std::setw(10) << backlog << " |\n"
		<< "\t| Req TimeOut (ms):  " << std::setw(10) << timeoutMs << " |\n"
		<< "\t=================================" << std::endl;
	
		if (!FailureMessages.empty())
			throw std::logic_error(FailureMessages);


		// me and the gpadi			
		std::cout << "	PASSED [Stress Testing]\n\n" << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << '\n' << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

	return (testNumber);
}

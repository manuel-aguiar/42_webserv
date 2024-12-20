

# include "../ListeningSocket.hpp"
# include "TestDependencies.hpp"
# include <iostream>

# include "../../../Globals/Globals.hpp"
# include "../../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerManager.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(void)
{

	/******************************************/

	//setup
	Globals				globals(NULL, NULL, NULL, NULL);
	Nginx_MemoryPool*	pool = Nginx_MemoryPool::create(4096, 1);
	ServerConfig		config(NULL, &globals);
	ServerManager		manager(config, globals);
	{
		ServerWorker		worker(manager, 0, *pool, globals);

		std::cout << "Test1: ";
		try
		{
			//setup
			t_addrinfo		addrInfo = (t_addrinfo){};

			//test
			ListeningSocket listener(worker, *pool, addrInfo, 10, globals);

			const int result = listener.open();
			const int expected = 0; // should fail

			if (result != expected)
				throw std::runtime_error("open() failed to return 0");

			std::cout << "	PASS\n";
		}
		catch(const std::exception& e)
		{
			std::cerr << "	FAILED: " << e.what() << '\n';
		}

		/**************************************************** */
		/**************************************************** */
		/**************************************************** */

		std::cout << "Test2: ";
		try
		{
			// setup
			t_addrinfo          						*res;
			t_addrinfo          						hints;
			int											status;

			t_addrinfo		  							cur;
			t_sockaddr_in								addr;

			hints = (t_addrinfo){};
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			status = getaddrinfo("127.0.0.1", "8080", &hints, &res);

			if (status != 0)
				throw std::runtime_error("getaddrinfo() failed" + std::string(::gai_strerror(errno)));

			std::memcpy(&cur, res, sizeof(t_addrinfo));
			std::memcpy(&addr, res->ai_addr, cur.ai_addrlen);
			cur.ai_addr = (t_sockaddr *)&addr;
			cur.ai_canonname = NULL;
			cur.ai_next = NULL;
			
			freeaddrinfo(res);


			//test
			ListeningSocket listener(worker, *pool, cur, 10, globals);
			


			const int result = listener.open();
			const int expected = 1; // should pass

			//cleanup
			

			if (result != expected)
				throw std::runtime_error("open() should pass with a valid addrinfo");

			std::cout << "	PASS\n";
		}
		catch(const std::exception& e)
		{
			std::cerr << "	FAILED: " << e.what() << '\n';
		}

		/**************************************************** */
		/**************************************************** */
		/**************************************************** */

		std::cout << "Test3: ";
		
		try
		{
			/*
				Testing getters	
			*/

			// setup
			t_addrinfo          						*res;
			t_addrinfo          						hints;
			int											status;
			t_addrinfo		  							cur;
			t_sockaddr_in								addr;	

			hints = (t_addrinfo){};
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			const char* ip = "127.0.0.1";
			const char* portStr = "8080";
			const int backlog = 10;
			t_port portNum = 8080;

			status = getaddrinfo(ip, portStr, &hints, &res);

			if (status != 0)
				throw std::runtime_error("getaddrinfo() failed" + std::string(::gai_strerror(errno)));

			std::memcpy(&cur, res, sizeof(t_addrinfo));
			std::memcpy(&addr, res->ai_addr, cur.ai_addrlen);
			cur.ai_addr = (t_sockaddr *)&addr;
			cur.ai_canonname = NULL;
			cur.ai_next = NULL;
			

			freeaddrinfo(res);

			//test
			ListeningSocket listener(worker, *pool, cur, backlog, globals);
			((t_sockaddr_in*)cur.ai_addr)->sin_port = ::ntohs(((t_sockaddr_in*)cur.ai_addr)->sin_port);

			{
				const int result = listener.open();
				const int expected = 1; // should pass
				if (result != expected)
					throw std::runtime_error("open() should pass with a valid addrinfo");
			}
			{
				const ServerWorker& result = listener.getWorker();
				const ServerWorker& expected = worker; 
				if (&result != &expected)
					throw std::runtime_error("getWorker doesn't retrieve the correct worker");
			}
			{
				const int result = listener.getSockType();
				const int expected = cur.ai_socktype; 
				if (result != expected)
					throw std::runtime_error("Wrong socktype");
			}
			{
				const int result = listener.getAddrlen();
				const int expected = cur.ai_addrlen; 
				if (result != expected)
					throw std::runtime_error("wrong addrlen");
			}
			{
				const t_sockaddr* result = listener.getAddr();
				const t_sockaddr* expected = cur.ai_addr;
				if (std::memcmp(result, expected, cur.ai_addrlen) != 0)
					throw std::runtime_error("addr not copied correctly");
			}
			{
				const t_port result = listener.getPort();
				const t_port expected = portNum; 
				if (result != expected)
					throw std::runtime_error("port not currectly converted to host byte order");
			}
			{
				const int result = listener.getBacklog();
				const int expected = backlog; 
				if (result != expected)
					throw std::runtime_error("backlog not set correctly");
			}
			{
				const t_socket result = listener.getSocket();
				const t_socket unexpected = -1;
				if (result == unexpected)
					throw std::runtime_error("getEvent() doesn't return the correct event");
			}
			const Event& event = listener.getEvent();
			{
				const t_func_event_handler result = event.getHandler();
				const t_func_event_handler expected = &ListeningSocket::EventCallbackAccept;
				if (result != expected)
					throw std::runtime_error("event should be listeningsocket::eventaccept");
			}
			{
				const t_ptr_event_data result = event.getData();
				const t_ptr_event_data expected = &listener;
				if (result != expected)
					throw std::runtime_error("event handler should be the listener itself");
			}
			listener.close();
			{
				const t_socket result = listener.getSocket();
				const t_socket expected = -1;
				if (result != expected)
					throw std::runtime_error("socket should be closed and = -1");
			}


			std::cout << "	PASS\n";
		}
		catch(const std::exception& e)
		{
			std::cerr << "	FAILED: " << e.what() << '\n';
		}
		
	}

	pool->destroy();

	return (0);
}


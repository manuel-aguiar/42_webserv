

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

# include "HttpManager/HttpManager.hpp"

class Connection;

class HttpModule
{
	public:
		HttpModule(ServerManager& serverManager);
		~HttpModule();

		// Module entry point
		static void 			initConnection(Connection* accepted);

		// getters
		const HttpManager& 		getHttpManager() const;
		const ServerManager& 	getServerManager() const;

		//no setters needed, Module needs a serverManager to start

		// accessors
		HttpManager& 			accessHttpManager();
		ServerManager& 			accessServerManager();

	private:
		HttpManager 			m_httpmanager;
		ServerManager& 			m_serverManager;

		HttpModule(const HttpModule& copy);
		HttpModule& operator=(const HttpModule& assign);

};

#endif

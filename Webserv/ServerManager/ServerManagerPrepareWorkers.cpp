/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerPrepareWorkers.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/04 12:45:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

// C++ headers



void	ServerManager::prepareWorkers()
{
	Nginx_MemoryPool*	workerMemPool;
	ServerWorker*		serverWorker;
	size_t				listenerCount;

	m_workers.reserve(m_config.getMaxWorkers());
	for (size_t i = 0; i < m_workers.size(); i++)
	{
		workerMemPool = Nginx_MemoryPool::create(4096, 1);
		serverWorker = (ServerWorker*)workerMemPool->allocate(sizeof (ServerWorker));
		new (serverWorker) ServerWorker(*this, i, workerMemPool, m_globals);
	}
}

int freeAllAddrInfo(std::vector<t_addrinfo*>& allAddrInfo)
{
	for (size_t i = 0; i < allAddrInfo.size(); ++i)
		::freeaddrinfo(allAddrInfo[i]);
}

int	setupAllAddrinfo(const ServerConfig& config, std::vector<t_addrinfo*>& allAddrInfo, int socktype, int addrFamily, int backlog)
{
	t_addrinfo          						*res;
	t_addrinfo          						*cur;
	t_addrinfo          						hints;
	int											status;
	int											totalCount = 0;
	
	typedef std::map<std::string, ServerBlocks> t_blocks;
	const t_blocks& 							blocks = config.getServerBlocks();

	hints = (t_addrinfo){};
	hints.ai_family = addrFamily;
	hints.ai_socktype = socktype;

	for (t_blocks::const_iterator iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		const std::vector<t_listeners>&	listeners = iter->second.getListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			status = getaddrinfo(
				listeners[i].first.c_str(), 
				listeners[i].second.c_str(),
				&hints,
				&res
			);
			if (status != 0)
			{
				// print something, maybe exit, return (-1) for error ??
				continue ;
			}
			allAddrInfo.push_back(res);
			for(cur = res; cur != NULL; cur = cur->ai_next)
				totalCount++;
		}
	}
	return (totalCount);
}

void	ServerManager::setupSingleListener(const t_ip_str& ip, const t_port_str& port, int socktype, int addrFamily, int backlog)
{
	ListeningSocket     *listener;
	t_addrinfo          hints;
	t_addrinfo          *res;
	t_addrinfo          *cur;
	t_sockaddr*         addr;

	hints = (struct addrinfo){};
	hints.ai_family = addrFamily;
	hints.ai_socktype = socktype;

	int status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);

	if (status != 0)
	{
		m_globals->logStatus("getaddrinfo(): " + std::string(gai_strerror(status)));
		return ;
	}

	for(cur = res; cur != NULL; cur = cur->ai_next)
	{
		listener = (ListeningSocket *)m_memPool->allocate(sizeof(ListeningSocket), true);
		new (listener) ListeningSocket(*this, m_globals);

		addr = (t_sockaddr *)m_memPool->allocate(cur->ai_addrlen, true);
		std::memcpy(addr, cur->ai_addr, cur->ai_addrlen);
		
		listener->setAddr(addr);
		listener->setSockType(cur->ai_socktype);
		listener->setProtocol(cur->ai_protocol);
		listener->setAddrlen(cur->ai_addrlen);
		listener->setBacklog(backlog);
		listener->setPort(::ntohs(((struct sockaddr_in *)cur->ai_addr)->sin_port));
		listener->accessEvent().setHandlerFunction_and_Data(&ListeningSocket::EventAccept, listener);
		listener->accessEvent().setFlags(EPOLLIN);


		if (listener->open())
			m_listeners.emplace_back(*listener);
		else
		{
			listener->~ListeningSocket();
			continue ;
		}
		//std::cout << "added listener: " << listener->m_sockfd << "and event fd " << listener->m_myEvent.m_fd << std::endl;
		m_eventManager.addEvent(listener->getSocket(), listener->getEvent());

	}
	freeaddrinfo(res);
	return (0);	
}

void	ServerManager::setupListeners()
{
	std::vector<t_addrinfo*> 	allAddrInfo;
	int							listenerCount;

	listenerCount = setupAllAddrinfo(m_config, allAddrInfo, SOCK_STREAM, AF_INET, 100);				///must be replaced with correct values
	
}


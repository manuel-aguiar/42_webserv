/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 15:51:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

// Toolkit Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"
# include "../../../Toolkit/List/List.hpp"
# include "../../../Toolkit/MemoryPool/MPool_FixedElem/MPool_FixedElem.hpp"

// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../Cgi_Definitions.h"
# include "../CgiRequestData/CgiRequestData.hpp"

// C++ headers
# include <map>

class CgiRequestData;
class Globals;

class CgiModule
{
	public:
		CgiModule(size_t workerCount, size_t backlogCount, Globals& globals);
		~CgiModule();

		// methods
		void									addInterpreter(const std::string& extension, const std::string& path);
		void									removeInterpreter(const std::string& extension);
		
		CgiRequestData*							acquireRequestData();
		void									executeRequest(CgiRequestData& data);
		void									finishRequest(CgiRequestData& data);
		void									cancelRequest(CgiRequestData& data);

		void									forceStop();

		//getters
		size_t									getBusyWorkerCount() const;
		const t_CgiEnvKey*						getBaseEnvKeys() const;
		const std::map<t_extension, t_path>&	getInterpreters() const;

	private:

		// internal classes, not exposed to the user
		class 			InternalCgiWorker;
		class			InternalCgiRequestData;

		size_t																		m_numWorkers;
		size_t																		m_backlog;
		size_t																		m_busyWorkerCount;

		// members fixed in place
		HeapArray<InternalCgiWorker>												m_allWorkers;
		HeapArray<InternalCgiRequestData>											m_allRequestData;
		
		// queues for available workers, request data and execution
		HeapCircularQueue<InternalCgiWorker*>										m_availableWorkers;
		HeapCircularQueue<InternalCgiRequestData*>									m_availableRequestData;
		HeapCircularQueue<InternalCgiRequestData*>									m_executionQueue;

		std::map<t_extension, t_path>												m_Interpreters;

		t_CgiEnvKey																	m_baseEnv[E_CGI_ENV_COUNT];

		Globals&																	m_globals;

		void																		mf_returnWorker(InternalCgiWorker& worker);
		void																		mf_returnRequestData(InternalCgiRequestData& data);
		void																		mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data);

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif
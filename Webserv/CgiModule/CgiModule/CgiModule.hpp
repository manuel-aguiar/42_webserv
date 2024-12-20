/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 13:13:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

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
		CgiModule(size_t workers, size_t backlog, Globals& globals);
		~CgiModule();

		// methods
		void									addInterpreter(const std::string& extension, const std::string& path);
		void									removeInterpreter(const std::string& extension);
		
		CgiRequestData*							acquireRequestData();
		void									executeRequest(CgiRequestData& data);
		void									cancelRequest(CgiRequestData& data);

		void									finishedReading(CgiRequestData& request);
		void									finishedWriting(CgiRequestData& request);
		void									finishedRequest(CgiRequestData& request);

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

		DynArray<InternalCgiWorker>													m_allWorkers;
		DynArray<InternalCgiRequestData>											m_allRequestData;
		
		
		DynArray<InternalCgiWorker*>												m_availableWorkers;
		DynArray<InternalCgiRequestData*>											m_availableRequestData;

		List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >	m_executionQueue;

		std::map<t_extension, t_path>												m_Interpreters;

		t_CgiEnvKey																	m_baseEnv[E_CGI_ENV_COUNT];

		Globals&																	m_globals;

		void																		mf_returnWorker(InternalCgiWorker& worker);
		void																		mf_returnRequestData(InternalCgiRequestData& data);

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif
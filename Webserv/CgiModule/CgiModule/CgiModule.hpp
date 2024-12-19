/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 11:37:52 by mmaria-d         ###   ########.fr       */
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

		void									forceStop();

		//getters
		size_t									getLiveRequestCount() const;
		const t_CgiEnvKey*						getBaseEnvKeys() const;
		const std::map<t_extension, t_path>&	getInterpreters() const;

	private:

		// internal classes, not exposed to the user
		class 			InternalCgiWorker;
		class			InternalCgiRequestData;

		size_t																		m_numWorkers;
		size_t																		m_backlog;
		size_t																		m_liveRequestCount;
		DynArray<InternalCgiWorker>													m_liveRequests;
		DynArray<InternalCgiRequestData>											m_allRequestData;
		
		
		// no memory pool for now
		List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >	m_spareRequestData;
		List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >	m_pendingRequests;
		List<InternalCgiWorker*, MPool_FixedElem<InternalCgiWorker**> >					m_spareLiveRequests;

		std::map<t_extension, t_path>												m_Interpreters;

		t_CgiEnvKey																	m_baseEnv[E_CGI_ENV_COUNT];

		Globals&																	m_globals;

		void																		mf_returnLiveRequest(InternalCgiWorker& request);
		void																		mf_deleteRequestData(InternalCgiRequestData& data);

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif
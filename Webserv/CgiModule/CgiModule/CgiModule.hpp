/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 11:10:47 by mmaria-d         ###   ########.fr       */
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
		
		CgiRequestData&							acquireRequestData();
		void									executeRequest(CgiRequestData& data);
		void									cancelRequest(CgiRequestData& data);

		void									forceStop();

		//getters
		size_t									getLiveRequestCount() const;
		const t_CgiEnvKey*						getBaseEnvKeys() const;
		const std::map<t_extension, t_path>&	getInterpreters() const;

	private:

		// internal request execution, other file since its long
		class 			CgiLiveRequest;

		// internal data handling
		class ManagedRequestData : public CgiRequestData
		{
			public:
				ManagedRequestData();
				~ManagedRequestData();
				ManagedRequestData(const ManagedRequestData &copy);
				ManagedRequestData &operator=(const ManagedRequestData &assign);

				void									setExecutor(CgiLiveRequest* const executor);
				void									setDataLocation(const List<ManagedRequestData>::iterator& location);
				void									setPendingLocation(const List<ManagedRequestData*>::iterator& location);

				CgiLiveRequest*							accessExecutor();
				List<ManagedRequestData>::iterator&		accessDataLocation();
				List<ManagedRequestData*>::iterator&	accessPendingLocation();

			private:
				CgiLiveRequest*							m_executor;
				List<ManagedRequestData>::iterator 		m_dataLocation;
				List<ManagedRequestData*>::iterator		m_pendingLocation;
		};

		size_t																		m_maxConnections;
		size_t																		m_liveRequestCount;
		DynArray<CgiLiveRequest>													m_liveRequests;
		List<ManagedRequestData>													m_allRequestData;
		
		
		// no memory pool for now
		List<ManagedRequestData*>													m_pendingRequests;
		List<CgiLiveRequest*>														m_spareLiveRequests;

		std::map<t_extension, t_path>												m_Interpreters;

		t_CgiEnvKey																	m_baseEnv[E_CGI_ENV_COUNT];

		Globals&																	m_globals;

		void																		mf_returnLiveRequest(CgiLiveRequest& request);
		void																		mf_deleteRequestData(ManagedRequestData& data);

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiRequestData.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:15:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 11:37:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALCGIREQUESTDATA_HPP

# define INTERNALCGIREQUESTDATA_HPP

// Project Headers
# include "../Cgi_Definitions.h"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiRequestData/CgiRequestData.hpp"

class CgiModule::InternalCgiRequestData : public CgiRequestData
{
	public:
		InternalCgiRequestData();
		~InternalCgiRequestData();
		InternalCgiRequestData(const InternalCgiRequestData &copy);
		InternalCgiRequestData &operator=(const InternalCgiRequestData &assign);

		void									    setExecutor(InternalCgiWorker* const executor);
		void									    setDataLocation(const List<InternalCgiRequestData>::iterator& location);
		void									    setPendingLocation(const List<InternalCgiRequestData*>::iterator& location);

		InternalCgiWorker*							    accessExecutor();
		List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator&		accessDataLocation();
		List<InternalCgiRequestData, MPool_FixedElem<InternalCgiRequestData> >::iterator&		accessPendingLocation();

	private:
		InternalCgiWorker*							m_executor;
		List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator 		m_dataLocation;
		List<InternalCgiRequestData, MPool_FixedElem<InternalCgiRequestData> >::iterator		m_pendingLocation;
};


#endif
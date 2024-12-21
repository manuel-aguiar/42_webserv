/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiRequestData.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:15:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 01:18:48 by mmaria-d         ###   ########.fr       */
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

		void		reset();

		void		setExecutor(InternalCgiWorker* const executor);
		void		setQueuePosition(const List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator& position);
		void		setReadFd(const t_fd fd);
		void		setWriteFd(const t_fd fd);


		InternalCgiWorker*							
					accessExecutor();
		List<InternalCgiRequestData *, MPool_FixedElem<InternalCgiRequestData *> >::iterator&		
					accessQueuePosition();


	private:
		InternalCgiWorker*							m_executor;
		
		List<InternalCgiRequestData *, MPool_FixedElem<InternalCgiRequestData *> >::iterator		
													m_queuePosition;
};


#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiRequestData.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:15:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 10:01:14 by mmaria-d         ###   ########.fr       */
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

		typedef enum
		{
			E_CGI_STATE_IDLE,
			E_CGI_STATE_ACQUIRED,
			E_CGI_STATE_QUEUED,
			E_CGI_STATE_EXECUTING,
			E_CGI_STATE_CANCELLED
		} 	t_CgiRequestState;

		InternalCgiRequestData();
		~InternalCgiRequestData();

		void					reset();

		void					setExecutor(InternalCgiWorker* const executor);
		void					setReadFd(const t_fd fd);
		void					setWriteFd(const t_fd fd);

		void					setState(const t_CgiRequestState state);
		t_CgiRequestState		getState() const;
		
		InternalCgiWorker*		accessExecutor();
		

	private:
		InternalCgiWorker*		m_executor;
		t_CgiRequestState		m_state;
		
		InternalCgiRequestData(const InternalCgiRequestData &copy);
		InternalCgiRequestData &operator=(const InternalCgiRequestData &assign);
};


#endif
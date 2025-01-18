/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiRequestData.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:15:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 20:08:43 by mmaria-d         ###   ########.fr       */
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

		void					reset();

		void					assignExecutor(InternalCgiWorker& executor);
		void					setMyTimer(const TimerTracker<Timer, InternalCgiRequestData*>::iterator& timer);
		TimerTracker<Timer, InternalCgiRequestData*>::iterator
								getMyTimer() const;

		void					setState(const t_CgiRequestState state);
		t_CgiRequestState		getState() const;
		
		InternalCgiWorker*		accessExecutor();
		
		t_ptr_CgiUser			getUser() const;
		t_func_CgiHandler		getHandler(const e_CgiCallback type) const;
		
		void					CallTheUser(const e_CgiCallback event);

		t_bytesRead				UserRead(t_fd readFd);
		t_bytesWritten			UserWrite(t_fd writeFd);

		
	private:

		friend class 		CgiModule;

		InternalCgiWorker*		m_executor;
		
		TimerTracker<Timer, InternalCgiRequestData*>::iterator	
								m_myTimer;
		
		InternalCgiRequestData(const InternalCgiRequestData &copy);
		InternalCgiRequestData &operator=(const InternalCgiRequestData &assign);
};


#endif
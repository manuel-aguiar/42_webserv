/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestUtils.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:25:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiLiveRequest.hpp"
# include "../../Globals/Globals.hpp"

void 	CgiModule::CgiLiveRequest::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("CgiLiveRequest::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
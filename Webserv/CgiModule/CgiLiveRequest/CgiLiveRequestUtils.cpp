/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestUtils.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:57:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiLiveRequest.hpp"
# include "../../Globals/Globals.hpp"

void 	CgiLiveRequest::closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("CgiLiveRequest::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
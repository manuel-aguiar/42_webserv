/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_Definitions.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:08:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 16:43:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIDEFINITIONS_H

# define CGIDEFINITIONS_H

typedef enum e_CgiEvents
{
	CGI_ON_READ,
	CGI_ON_WRITE,
	CGI_ON_ERROR,
	CGI_ON_CLOSE,
	CGI_ON_TIMEOUT,
	CGI_EVENT_COUNT
} t_CgiEvents;

#endif
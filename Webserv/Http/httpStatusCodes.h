/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpStatusCodes.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:37:57 by manuel            #+#    #+#             */
/*   Updated: 2024/11/15 08:30:07 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODES_H

# define HTTPSTATUSCODES_H

typedef enum e_http_parsing_status
{
	HTTP_PARSING_OK = 0,
	HTTP_PARSING_ERROR = -1
}	t_http_parsing_status;

typedef enum e_http_status
{
	HTTP_OK = 200,
	HTTP_CREATED = 201
	//...
}	t_http_status;

#endif

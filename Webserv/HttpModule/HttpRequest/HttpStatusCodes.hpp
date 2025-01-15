/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodes.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:25:11 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/07 22:25:38 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPSTATUSCODES_HPP
#define HTTPSTATUSCODES_HPP

# define WEBSRV_OK 200
# define WEBSRV_CREATED 201
# define WEBSRV_ACCEPTED 202
# define WEBSRV_NO_CONTENT 204

# define WEBSRV_MOVED_PERMANENTLY 301
# define WEBSRV_FOUND 302
# define WEBSRV_NOT_MODIFIED 304
# define WEBSRV_TEMPORARY_REDIRECT 307

# define WEBSRV_BAD_REQUEST 400
# define WEBSRV_UNAUTHORIZED 401
# define WEBSRV_FORBIDDEN 403
# define WEBSRV_NOT_FOUND 404
# define WEBSRV_METHOD_NOT_ALLOWED 405
# define WEBSRV_NOT_ACCEPTABLE 406
# define WEBSRV_REQUEST_TIMEOUT 408
# define WEBSRV_CONFLICT 409
# define WSRV_INTERNAL_SERVER_ERROR 500

# define WEBSRV_NOT_IMPLEMENTED 501
# define WEBSRV_BAD_GATEWAY 502
# define WEBSRV_SERVICE_UNAVAILABLE 503
# define WEBSRV_GATEWAY_TIMEOUT 504
# define WEBSRV_HTTP_VERSION_NOT_SUPPORTED 505

#endif
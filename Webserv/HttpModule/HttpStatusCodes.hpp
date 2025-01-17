/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodes.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:37:57 by manuel            #+#    #+#             */
/*   Updated: 2025/01/16 21:09:19 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODES_H
# define HTTPSTATUSCODES_H

# define HTTP_OK 200
# define HTTP_CREATED 201
# define HTTP_ACCEPTED 202
# define HTTP_NO_CONTENT 204
# define HTTP_RESET_CONTENT 205
# define HTTP_PARTIAL_CONTENT 206

# define HTTP_MULTIPLE_CHOICES 300
# define HTTP_MOVED_PERMANENTLY 301
# define HTTP_FOUND 302
# define HTTP_SEE_OTHER 303
# define HTTP_NOT_MODIFIED 304
# define HTTP_TEMPORARY_REDIRECT 307
# define HTTP_PERMANENT_REDIRECT 308

# define HTTP_BAD_REQUEST 400
# define HTTP_METHOD_NOT_ALLOWED 405
# define HTTP_URI_TOO_LONG 414
# define HTTP_HEADERS_TOO_LARGE 431
# define HTTP_PRECONDITION_FAILED 412
# define HTTP_PAYLOAD_TOO_LARGE 413
# define HTTP_UNSUPPORTED_MEDIA_TYPE 415
# define HTTP_TOO_MANY_REQUESTS 429
# define HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE 431
# define HTTP_UNAVAILABLE_FOR_LEGAL_REASONS 451

# define HTTP_INTERNAL_ERROR 500
# define HTTP_NOT_IMPLEMENTED 501
# define HTTP_BAD_GATEWAY 502
# define HTTP_SERVICE_UNAVAILABLE 503
# define HTTP_GATEWAY_TIMEOUT 504
# define HTTP_HTTP_VERSION_NOT_SUPPORTED 505
# define HTTP_VARIANT_ALSO_NEGOTIATES 506
# define HTTP_INSUFFICIENT_STORAGE 507
# define HTTP_LOOP_DETECTED 508
# define HTTP_NOT_EXTENDED 510
# define HTTP_NETWORK_AUTHENTICATION_REQUIRED 511

#endif

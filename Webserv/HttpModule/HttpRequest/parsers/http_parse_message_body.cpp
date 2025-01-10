/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parse_message_body.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:15:00 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/16 21:38:18 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HttpRequest.hpp"


int HttpRequest::mf_parseBody(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}

int HttpRequest::mf_parseChunkedBody(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}
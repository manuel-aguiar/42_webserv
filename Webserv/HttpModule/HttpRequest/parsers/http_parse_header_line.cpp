/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parse_header_line.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:15:00 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/16 21:38:14 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HttpRequest.hpp"

int HttpRequest::mf_parseHeaders(const std::string& data)
{
    (void)data;
    return RequestStatus::OK;
}
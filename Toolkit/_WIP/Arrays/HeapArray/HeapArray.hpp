/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:Webserv/HttpModule/HttpRequest/parsers/http_parse_message_body.cpp
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
========
/*   HeapArray.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:58:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 09:09:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPARRAY_HPP

# define HEAPARRAY_HPP

template <typename T>
class HeapArray;

# include "HeapArray.tpp"

#endif
>>>>>>>> main:Toolkit/_WIP/Arrays/HeapArray/HeapArray.hpp

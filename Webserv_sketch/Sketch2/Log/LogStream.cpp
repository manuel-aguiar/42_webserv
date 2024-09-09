/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogStream.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:28:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:39:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogStream.hpp"

LogStream::LogStream(std::ostream& stream) : _stream(stream)
{

}

LogStream::~LogStream()
{

}

void    LogStream::record(const std::string& entry)
{
    _stream << entry;
}

void    LogStream::record(const char* entry)
{
    _stream << entry;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParameterException.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:04:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 09:04:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParameterException.hpp"

ParameterException::ParameterException(const char* caller, const char* syscall, const char* strerror) :
    _totalWritten(0),
    _curLen(0),
    _curWriteSize(0),
    _buffer()
{
    _placeInBuffer(caller);
    _placeInBuffer(": ");
    _placeInBuffer(syscall);
    _placeInBuffer("(): ");
    _placeInBuffer(strerror);
    _buffer[_totalWritten] = '\0';
}

ParameterException::ParameterException(const ParameterException& copy) :
    _totalWritten(copy._totalWritten),
    _curLen(0),
    _curWriteSize(0)
{
    std::memcpy(_buffer, copy._buffer, sizeof(_buffer));
}

ParameterException::~ParameterException() throw() {}

const char* ParameterException::what() const throw()
{
    return (_buffer);
}

void    ParameterException::_placeInBuffer(const char* str)
{
    _curLen = strlen(str);
    _curWriteSize = std::min(sizeof(_buffer) - _totalWritten - 1, _curLen);
    std::memcpy(&_buffer[_totalWritten], str, _curWriteSize);
    _totalWritten += _curWriteSize;
}



ParameterException::ParameterException() {}

ParameterException& ParameterException::operator=(const ParameterException& assign) {(void)assign; return (*this);}  
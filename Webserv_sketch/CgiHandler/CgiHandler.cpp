/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:58:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

Cgi_Handler::Cgi_Handler() : 
    _queue(MemoryPool_Dealloc<t_cgi_data>(4096, 5, 5)),
    _maxConcurrent(MAX_CONCURRENT_CGI), 
    _currentConcurrent(0)
{
    
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:37:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI_Handler.hpp"

CGI_Handler::CGI_Handler() : 
    _queue(MemoryPool_Dealloc<t_CGI_data>(4096, 5, 5)),
    _maxConcurrent(MAX_CONCURRENT_CGI), 
    _currentConcurrent(0)
{
    
}
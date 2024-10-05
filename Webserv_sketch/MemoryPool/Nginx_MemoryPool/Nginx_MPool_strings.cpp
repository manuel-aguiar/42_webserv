/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool_strings.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 13:38:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 13:41:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"
#include <cstring>

char*        Nginx_MemoryPool::strdup(Nginx_MemoryPool& pool, const char* str)
{
    size_t len; 
    char* newStr;

    len = std::strlen(str);
    newStr = (char*)pool.allocate(len + 1, false);
    std::memcpy(newStr, str, len);
    newStr[len] = '\0';
    
    return (newStr);
}
char*        Nginx_MemoryPool::strjoin(Nginx_MemoryPool& pool, const char* str1, const char* str2)
{
    size_t len1;
    size_t len2;
    char* newStr;

    len1 = std::strlen(str1);
    len2 = std::strlen(str2);
    newStr = (char*)pool.allocate(len1 + len2 + 1, false);
    std::memcpy(newStr, str1, len1);
    std::memcpy(newStr + len1, str2, len2);
    newStr[len1 + len2] = '\0';
    
    return (newStr);
}
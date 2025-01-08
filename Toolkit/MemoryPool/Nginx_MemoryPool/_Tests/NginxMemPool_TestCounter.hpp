/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxMemPool_TestCounter.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:28:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:14:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINXMEMPOOL_TESTCOUNTER_HPP

# define NGINXMEMPOOL_TESTCOUNTER_HPP

# include "../Nginx_MemoryPool.hpp"

struct AllocationStats
{
    size_t  m_allocCount;
    size_t  m_allocBytes;
    
    size_t  m_deleteCount;
    size_t  m_newCount;
};

class NginxMemPool_TestCounter : public Nginx_MemoryPool
{
    public:
        NginxMemPool_TestCounter(size_t blockSize, size_t startingBlocks, AllocationStats* placeStats);
        ~NginxMemPool_TestCounter();

		void*       allocate(size_t size);
        void*       allocate(size_t size, size_t alignment);
        void        reset(int maxBlocks = INT_MAX);
        void        destroy();        
    private:

        AllocationStats*  m_stats;

        NginxMemPool_TestCounter();
        NginxMemPool_TestCounter(const NginxMemPool_TestCounter& copy);
        NginxMemPool_TestCounter& operator=(const NginxMemPool_TestCounter& copy);
};


#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_FixedBlock.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:17:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_FIXEDPOOL_HPP

# define NGINX_FIXEDPOOL_HPP

// C++ headers
# include <cstddef>
# include <cassert>

typedef struct sockaddr_storage t_sockaddr;

typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

class HeapFixedBlock
{
    public:
        HeapFixedBlock(size_t blockSize);
        ~HeapFixedBlock();

		void* 						allocate(size_t size);
        void*                       allocate(size_t size, size_t alignment);
        void                        reset();
        void                        destroy();

        size_t                      getBlockSize() const;
        size_t                      getFreeSpace() const;

    private:
    
        t_byte*                     m_array;
        t_byte*                     m_freePosition;
        t_byte*                     m_endOfBlock;
        const size_t                m_blockSize;

        
        // private copy and assignment and default construction
        HeapFixedBlock();
        HeapFixedBlock(const HeapFixedBlock& pool);
        HeapFixedBlock& operator=(const HeapFixedBlock& pool);

        // helper functions

        static t_byte*      mf_allignedAlloc(void *byte, size_t alignment);

};


#endif

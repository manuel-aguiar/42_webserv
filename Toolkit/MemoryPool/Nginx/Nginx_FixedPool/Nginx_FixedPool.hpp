/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_FixedPool.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 16:28:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_FIXEDPOOL_HPP

# define NGINX_FIXEDPOOL_HPP

// C++ headers
# include <limits>
# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>

typedef struct sockaddr_storage t_sockaddr;

typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

class Nginx_FixedPool
{
    public:
        Nginx_FixedPool(size_t blockSize);
        ~Nginx_FixedPool();

		void* 						allocate(size_t size);
        void*                       allocate(size_t size, size_t alignment);
        void                        reset();
        void                        destroy();

    private:
    
        t_byte*                     m_array;
        t_byte*                     m_freePosition;
        t_byte*                     m_endOfBlock;
        const size_t                m_blockSize;

        
        // private copy and assignment and default construction
        Nginx_FixedPool();
        Nginx_FixedPool(const Nginx_FixedPool& pool);
        Nginx_FixedPool& operator=(const Nginx_FixedPool& pool);

        // helper functions

        static t_byte*      mf_allignedAlloc(void *byte, size_t alignment);

};


#endif

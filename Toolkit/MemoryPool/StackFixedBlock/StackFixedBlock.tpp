/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackFixedBlock.tpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:59:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKFIXEDBLOCK_TPP

# define STACKFIXEDBLOCK_TPP

// C++ headers
# include <cstddef>
# include <cassert>




/*

    Nginx-style non deallocating pool

*/
template <size_t BlockSize>
class StackFixedBlock
{
    public:
        StackFixedBlock() :
            m_freePosition(m_array),
            m_endOfBlock(m_array + BlockSize) {}

        ~StackFixedBlock() {}

    
		void* 						allocate(size_t size)
        {
            return (allocate(size, (size > sizeof(size_t)) ? sizeof(size_t) : size));
        }

        void*                       allocate(size_t size, size_t alignment)
        {
            t_byte* location = mf_allignedAlloc(m_freePosition, alignment);

            assert(location + size <= m_endOfBlock);

            m_freePosition = (t_byte*)((size_t)location + size);
            return (location);
        }

        void                        reset()
        {
            m_freePosition = m_array;
        }

        size_t                      getBlockSize() const
        {
            return (BlockSize);
        }
        size_t                      getFreeSpace() const
        {
            return (m_endOfBlock - m_freePosition);
        }

    private:
    
        typedef unsigned char       t_byte;
        t_byte                      m_array[BlockSize];
        t_byte*                     m_freePosition;
        t_byte*                     m_endOfBlock;

        
        StackFixedBlock(const StackFixedBlock& pool) {(void)pool;}
        StackFixedBlock& operator=(const StackFixedBlock& pool){(void)pool; return (*this);}

        static t_byte*      mf_allignedAlloc(void *byte, size_t alignment)
        {
            return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
        }

};


#endif

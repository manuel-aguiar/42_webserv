

#ifndef SLABIMPL_HPP

# define SLABIMPL_HPP

# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <limits>
# include <cstddef>


class SlabImpl
{
    public:

        template <typename T>
        T* allocate();
        
        template <typename T>
        void deallocate(T* p);

        template <typename T>
        void destroy(T* p);

        size_t size() const;
        size_t capacity() const;
        size_t getElemCount() const;
        
    protected:
        union s_Slot
        {
            s_Slot* m_next;
        };

        typedef char*       t_data_pointer;
        typedef s_Slot      t_slot_type;
        typedef s_Slot*     t_slot_pointer;
        
        SlabImpl(t_slot_pointer array, size_t nodeSize, size_t capacity);
        SlabImpl(const SlabImpl& memoryPool);
        SlabImpl& operator=(const SlabImpl& assign);
        ~SlabImpl();

        void    reset(t_slot_pointer array, size_t nodeSize, size_t capacity);

    private:
        t_slot_pointer  m_begin;
        size_t          m_nodeSize;
        size_t          m_elemCount;
        size_t          m_capacity;
        t_slot_pointer  m_freeSlot;

};

template <typename T>
T* SlabImpl::allocate()
{
    ASSERT_EQUAL(sizeof(T) <= m_nodeSize, true, "SlabImpl: Size of T is greater than nodeSize");
    ASSERT_EQUAL(m_elemCount < m_capacity, true, "SlabImpl: Out of memory");

    if (m_freeSlot != NULL)
    {
        T* result = reinterpret_cast<T*>(m_freeSlot);
        m_freeSlot = m_freeSlot->m_next;
        m_elemCount++;
        return result;
    }
    else
    {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(m_begin) + (m_nodeSize * m_elemCount++));
    }
}

template <typename T>
void SlabImpl::deallocate(T* p)
{
    ASSERT_EQUAL(sizeof(T) <= m_nodeSize, true, "SlabImpl: Size of T is greater than nodeSize");

    if (p != NULL)
    {
        reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
        m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
        --m_elemCount;
    }
}

template <typename T>
void SlabImpl::destroy(T* p)
{
    if (p != NULL)
        p->~T();
}

#endif
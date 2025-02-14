

#ifndef OBJECTPOOLIMPL_TPP

# define OBJECTPOOLIMPL_TPP

# include <cstddef>

template <typename T>
class ObjectPoolImpl
{
	public:


		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		

		size_t		getElemCount() const { return (m_elemCount); }

        size_t      capacity() const { return (m_maxElems); }

		pointer address(reference x) const
		{
			return (&x);
		}

		const_pointer address(const_reference x) const
		{
			return (&x);
		}

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			(void)hint;
			(void)n;
            
			ASSERT_EQUAL(m_elemCount < m_maxElems, true, "ObjectPoolImpl: Out of memory");
			if (m_freeSlot != 0)
			{
				pointer result = reinterpret_cast<pointer>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return (result);
			}
			else
				return reinterpret_cast<pointer>(&m_begin[m_elemCount++]);
		}

		void deallocate(pointer p, size_type n = 1)
		{
			(void)n;
			if (p == NULL)
				return ;

			reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
			m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
			--m_elemCount;
		}

		int getElementSize() const { return (sizeof(T)); }

		size_type max_size() const
		{
			return (m_maxElems);
		}

		void construct(pointer p, const_reference val)
		{
			new (p) value_type (val);
		}
		
		void destroy(pointer p)
		{
			p->~value_type();
		}

		pointer newElement(const_reference val)
		{
			pointer result = allocate();
			construct(result, val);
			return result;
		}

		void deleteElement(pointer p)
		{
			if (p != 0)
			{
				p->~value_type();
				deallocate(p);
			}
		}

    protected:

		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			static const size_t value = (TSize + Alignment - 1) & ~(Alignment - 1);
		};



		union s_Slot
		{
			char			m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*			m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

        template <typename U> struct rebind {
			typedef ObjectPoolImpl<U> other;
		};

		ObjectPoolImpl(t_slot_pointer begin, size_t maxElems) :
			m_begin(begin),
			m_elemCount(0),
			m_maxElems(maxElems),
			m_freeSlot(NULL) {}

		ObjectPoolImpl(const ObjectPoolImpl& copy)  :
			m_begin(copy.m_begin),
			m_elemCount(copy.m_elemCount),
			m_maxElems(copy.m_maxElems),
			m_freeSlot(copy.m_freeSlot) {}

        template<class U>
        ObjectPoolImpl(const ObjectPoolImpl<U>& rebind) :
            m_begin(NULL),
            m_elemCount(0),
            m_maxElems(rebind.capacity()),
            m_freeSlot(NULL)
         {(void)rebind;}

        ObjectPoolImpl& operator=(const ObjectPoolImpl& assign)
        {
            if (this == &assign)
                return (*this);

            m_begin = assign.m_begin;
            m_elemCount = assign.m_elemCount;
            m_maxElems = assign.m_maxElems;
            m_freeSlot = assign.m_freeSlot;

            return (*this);
        }

		~ObjectPoolImpl() {}

        void reset(t_slot_pointer begin, size_t maxElems)
        {
            m_begin = begin;
            m_elemCount = 0;
            m_maxElems = maxElems;
            m_freeSlot = NULL;
        }

        void assignArray(t_slot_pointer begin)
        {
            m_begin = begin;
        }

		t_slot_pointer 				m_begin;
		size_t 						m_elemCount;
		size_t 						m_maxElems;
		t_slot_pointer 				m_freeSlot;

		bool operator==(const ObjectPoolImpl& other) const
		{ return (m_begin == other.m_begin); }
		bool operator!=(const ObjectPoolImpl& other) const
		{ return (m_begin != other.m_begin); }
};


#endif
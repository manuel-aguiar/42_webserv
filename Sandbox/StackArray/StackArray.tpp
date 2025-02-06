

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

// Project headers
# include "../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <cstddef>
#include <iterator>


template <typename T>
class StackArrayImpl
{
    public:
        class Iterator;
        typedef StackArrayImpl::Iterator iterator;

        iterator begin()    { return iterator(m_begin); }
        iterator end()      { return iterator(m_end); }

        void    push_back(const T& value)
        {
            ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
            new (m_end++) T(value);
        }

        void    pop_back()
        {
            ASSERT_EQUAL(size() != 0, true, "StackArray pop: Index out of bounds");
            m_end--;
            m_end->~T();
        }

        size_t size() const { return (size_t)(m_end - m_begin); }
        size_t capacity() const { return (m_capacity); }



		T& operator[](const size_t index)
		{
            ASSERT_EQUAL(index < size(), true, "StackArray operator[]: Index out of bounds");
			return (m_begin[index]);
		}

        const T& operator[](const size_t index) const
		{
			ASSERT_EQUAL(index < size(), true, "StackArray operator[]: Index out of bounds");
			return (m_begin[index]);
		}

        T& at(size_t index)
        {
            ASSERT_EQUAL(index < size(), true, "StackArray Copy: Index out of bounds");
            return (m_begin[index]);
        }

		T& front()
		{
            ASSERT_EQUAL(size() != 0, true, "StackArray Copy: Index out of bounds");
			return (m_begin[0]);
		}

		T& back()
		{
            ASSERT_EQUAL(size() != 0, true, "StackArray Copy: Index out of bounds");
			return (m_end[-1]);
		}

		void emplace_back()
        {
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T();
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1, arg2, arg3);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			ASSERT_EQUAL(size() < m_capacity, true, "StackArray push: Index out of bounds");
			new (m_end++) T(arg1, arg2, arg3);
        }

    protected:
        StackArrayImpl(T* begin, T* end, const size_t capacity) : m_begin(begin), m_end(end), m_capacity(capacity) {}
        StackArrayImpl(T* begin, T* end, const size_t capacity, const T& value) : m_begin(begin), m_end(end), m_capacity(capacity)
        {
            for (size_t i = 0; i < m_capacity; i++)
                new (&m_begin[i]) T(value);
        }


        ~StackArrayImpl()
        {
            for (T* ptr = m_begin; ptr != m_end; ++ptr)
                ptr->~T();
        }

        StackArrayImpl(const StackArrayImpl& other) : 
            m_begin(other.m_begin), 
            m_end(other.m_end), 
            m_capacity(other.m_capacity) {}

        StackArrayImpl& operator=(const StackArrayImpl& other)
        {

			if (this == &other)
				return (*this);
            
            size_t mySize = size();
            size_t otherSize = other.size();
            size_t smaller = (mySize < otherSize) ? mySize : otherSize;
            
            for (size_t i = 0; i < smaller; i++)
                m_begin[i] = other.m_begin[i];
                
            if (smaller == mySize)
            {
                for (size_t i = mySize; i < otherSize; i++)
                    new (&m_begin[i]) T(other.m_begin[i]);
            }
            else
            {
                for (size_t i = otherSize; i < mySize; i++)
                    m_begin[i].~T();
            }
            
            m_begin = other.m_begin;
            m_end = other.m_end;

			return (*this);
        }

    private:
        T*              m_begin;
        T*              m_end;
        const size_t    m_capacity;
    
    public:
        class Iterator
		{
            public:
                typedef T                                   value_type;
                typedef T*                                  pointer;
                typedef T&                                  reference;
                typedef std::ptrdiff_t                      difference_type;
                typedef std::random_access_iterator_tag     iterator_category;

                Iterator(pointer ptr) : m_ptr(ptr) {}
                Iterator(const iterator& other) : m_ptr(other.m_ptr) {}
                ~Iterator() {}
                Iterator& operator=(const iterator& other) { m_ptr = other.m_ptr; return *this; }


                reference operator*() const { return *m_ptr; }
                pointer operator->() const { return m_ptr; }

                Iterator& operator++()
                {
                    ++m_ptr;
                    return *this;
                }

                Iterator operator++(int)
                {
                    iterator tmp = *this;
                    ++m_ptr;
                    return tmp;
                }

                Iterator& operator--()
                {
                    --m_ptr;
                    return *this;
                }

                Iterator operator--(int)
                {
                    iterator tmp = *this;
                    --m_ptr;
                    return tmp;
                }

                bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }
                bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }


                reference operator[](size_t index) const { return *(m_ptr + index); }
                Iterator operator+(size_t n) const { return Iterator(m_ptr + n); }
                Iterator operator-(size_t n) const { return Iterator(m_ptr - n); }
                difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }

                Iterator& operator+=(size_t n)
                {
                    m_ptr += n;
                    return *this;
                }
                Iterator& operator-=(size_t n)
                {
                    m_ptr -= n;
                    return *this;
                }

            private:
                pointer m_ptr;
        };
};

template <typename T, size_t ElemCount>
class StackArray : public StackArrayImpl<T>
{
	public:
		StackArray() : 
            StackArrayImpl<T>(reinterpret_cast<T*>(m_array), reinterpret_cast<T*>(m_array), ElemCount),
            m_internalArray(reinterpret_cast<T*>(m_array)) {}


        StackArray(const T& value) : 
            StackArrayImpl<T>(reinterpret_cast<T*>(m_array), reinterpret_cast<T*>(m_array) + ElemCount, ElemCount, value),
            m_internalArray(reinterpret_cast<T*>(m_array)) {}

		~StackArray() {};

		StackArray(const StackArray &other) : 
            StackArrayImpl<T>(reinterpret_cast<T*>(m_array), reinterpret_cast<T*>(m_array) + other.size(), ElemCount),
            m_internalArray(reinterpret_cast<T*>(m_array)) 
		{
			*this = other;
		};

		StackArray &operator=(const StackArray &other)
		{
            if (this == &other)
                return (*this);

            StackArrayImpl<T>::operator=(other);

            return (*this);
		};


        T* getArray() const {return (m_internalArray);}






	private:
		typedef unsigned char 		t_byte;
        T*                          m_internalArray;
		t_byte  					m_array[sizeof(T) * ElemCount];
};




#endif
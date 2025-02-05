

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <cstddef>
#include <iterator>

template <typename T, size_t ElemCount>
class StackArray
{
	public:
		StackArray() : m_size(0), m_internalArray(reinterpret_cast<T*>(m_array)) {}


        StackArray(const T& value) : m_size(ElemCount), m_internalArray(reinterpret_cast<T*>(m_array))
        {
            for (size_t i = 0; i < m_size; i++)
                new (&m_internalArray[i]) T(value);
        }

		~StackArray()
		{
			for (size_t i = 0; i < m_size; i++)
				m_internalArray[i].~T();
		};

		StackArray(const StackArray &other) : m_size(0), m_internalArray(reinterpret_cast<T*>(m_array)) 
		{
			*this = other;
		};

		StackArray &operator=(const StackArray &other)
		{
            
			if (this == &other)
				return (*this);
            
            size_t smaller = (m_size < other.m_size) ? m_size : other.m_size;
            for (size_t i = 0; i < smaller; i++)
                m_internalArray[i] = other.m_internalArray[i];
                
            if (smaller == m_size)
            {
                for (size_t i = m_size; i < other.m_size; i++)
                    new (&m_internalArray[i]) T(other.m_internalArray[i]);
            }
            else
            {
                for (size_t i = other.m_size; i < m_size; i++)
                    m_internalArray[i].~T();
            }

            m_size = other.m_size;
			return (*this);
		};

		T& operator[](const size_t index)
		{
            ASSERT_EQUAL(index < ElemCount, true, "StackArray Copy: Index out of bounds");
			return (m_internalArray[index]);
		}

        const T& operator[](const size_t index) const
		{
			ASSERT_EQUAL(index < ElemCount, true, "StackArray Copy: Index out of bounds");
			return (m_internalArray[index]);
		}

		
		size_t size() const
		{
			return (m_size);
		}

		size_t capacity() const
		{
				return (ElemCount);
		}

        T* getArray() const {return (m_internalArray);}


        T& at(size_t index)
        {
            ASSERT_EQUAL(this->m_size != 0 && index < this->m_size, true, "StackArray Copy: Index out of bounds");
            return (m_internalArray[index]);
        }

		T& front()
		{
            ASSERT_EQUAL(this->m_size != 0, true, "StackArray Copy: Index out of bounds");
			return (m_internalArray[0]);
		}

		T& back()
		{
            ASSERT_EQUAL(this->m_size != 0, true, "StackArray Copy: Index out of bounds");
			return (m_internalArray[(m_size - 1)]);
		}

		void push_back(const T& value)
		{
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(value);
		}

        void pop_back()
        {
			ASSERT_EQUAL(this->m_size != 0, true, "StackArray Copy: Index out of bounds");
            m_internalArray[(m_size-- - 1)].~T();
        }

		void emplace_back()
        {
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T();
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1, arg2, arg3);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			ASSERT_EQUAL(this->m_size < ElemCount, true, "StackArray Copy: Index out of bounds");
			new (m_internalArray + m_size++) T(arg1, arg2, arg3);
        }		

		class iterator
		{
            public:
                typedef T                                   value_type;
                typedef T*                                  pointer;
                typedef T&                                  reference;
                typedef std::ptrdiff_t                      difference_type;
                typedef std::random_access_iterator_tag     iterator_category;

                iterator(pointer ptr) : m_ptr(ptr) {}
                iterator(const iterator& other) : m_ptr(other.m_ptr) {}
                ~iterator() {}
                iterator& operator=(const iterator& other) { m_ptr = other.m_ptr; return *this; }


                reference operator*() const { return *m_ptr; }
                pointer operator->() const { return m_ptr; }

                iterator& operator++()
                {
                    ++m_ptr;
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator tmp = *this;
                    ++m_ptr;
                    return tmp;
                }

                iterator& operator--()
                {
                    --m_ptr;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator tmp = *this;
                    --m_ptr;
                    return tmp;
                }

                bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
                bool operator!=(const iterator& other) const { return m_ptr != other.m_ptr; }


                reference operator[](size_t index) const { return *(m_ptr + index); }
                iterator operator+(size_t n) const { return iterator(m_ptr + n); }
                iterator operator-(size_t n) const { return iterator(m_ptr - n); }
                difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }

                iterator& operator+=(size_t n)
                {
                    m_ptr += n;
                    return *this;
                }
                iterator& operator-=(size_t n)
                {
                    m_ptr -= n;
                    return *this;
                }

            private:
                pointer m_ptr;
        };

    iterator begin() { return iterator(&m_internalArray[0]); }
    iterator end() { return iterator(&m_internalArray[m_size]); }

	private:
		typedef unsigned char 		t_byte;
		size_t 						m_size;
        T*                          m_internalArray;
		t_byte  					m_array[sizeof(T) * ElemCount];
};




#endif
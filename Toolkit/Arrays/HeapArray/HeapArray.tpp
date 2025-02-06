

#ifndef HEAPARRAY_TPP

# define HEAPARRAY_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstdlib>
# include <cassert>
# include <memory>
# include <cstring>

template <typename T, typename Allocator>
class HeapArray
{
	public:

        template <typename U>
        class ArrayIterator;
        
        typedef ArrayIterator<T>         iterator;
        typedef ArrayIterator<const T>   const_iterator;
        typedef ArrayIterator<T>         reverse_iterator;
        typedef ArrayIterator<const T>   const_reverse_iterator;

		HeapArray(const size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)), 
			m_size(0), 
			m_capacity(capacity)
		{
			ASSERT_EQUAL(capacity != 0, true, "HeapArray: capacity must be greater than 0");
		}

		HeapArray(const size_t capacity, const T& value, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)), 
			m_size(capacity), 
			m_capacity(capacity)
		{
			ASSERT_EQUAL(capacity != 0, true, "HeapArray: capacity must be greater than 0");
			for (size_t i = 0; i < m_size; i++)
				m_allocator.construct(&m_array[i], T(value));
		}


		HeapArray(const HeapArray &other) : 
			m_allocator(other.m_allocator),
			m_array(m_allocator.allocate(other.m_capacity)), 
			m_size(0), 
			m_capacity(other.m_capacity)
		{
			*this = other;
		}

		~HeapArray()
		{
			for (size_t i = 0; i < m_size; i++)
				m_allocator.destroy(&m_array[i]);
			m_allocator.deallocate(m_array, m_capacity);
		}

		HeapArray &operator=(const HeapArray &other)
		{
			ASSERT_EQUAL(m_capacity, other.m_capacity, "HeapArray Copy: capacity of both instances must be equal");
			
			if (this == &other)
				return (*this);

            if (m_allocator != other.m_allocator)
            {
                clear();
               	m_allocator.deallocate(m_array, m_capacity);
                m_allocator = other.m_allocator;
				m_array = m_allocator.allocate(m_capacity);
            }

			size_t smaller = (m_size < other.m_size) ? m_size : other.m_size;
			for (size_t i = 0; i < smaller; ++i)
				m_array[i] = other.m_array[i];
			
			if (smaller == m_size)
			{
				for (size_t i = smaller; i < other.m_size; ++i)
					m_allocator.construct(&m_array[i], other.m_array[i]);
			}
			else
			{
				for (size_t i = smaller; i < m_size; ++i)
					m_allocator.destroy(&m_array[i]);
			}

			m_size = other.m_size;

			return (*this);
		}

		void move(HeapArray& from)
		{
			clear();
			m_allocator.deallocate(&m_array[0], m_capacity);

			m_array = from.m_array;
			m_size = from.m_size;
			from.m_array = NULL;
			from.m_size = 0;
		}

		T& operator[](const size_t index)
		{
			ASSERT_EQUAL(index < m_capacity, true, std::string("HeapArray: Index out of bounds: ") + TestHelpers::to_string(index));
			assert(index < m_capacity);
			return (m_array[index]);
		}

		const T& operator[](const size_t index) const
		{
			ASSERT_EQUAL(index < m_capacity, true, "HeapArray: Index out of bounds");
			return (m_array[index]);
		}

		size_t size() const
		{
			return (m_size);
		}

		size_t capacity()
		{
			return (m_capacity);
		}

		void clear()
		{
			for (size_t i = 0; i < m_size; i++)
				m_allocator.destroy(&m_array[i]);
			m_size = 0;
		}

		const Allocator& getAllocator() const
		{
			return (m_allocator);
		}

        T* getArray() const {return (m_array);}


        T& at(size_t index)
        {
            ASSERT_EQUAL(index != 0 && index < m_capacity, true, "HeapArray Copy: Index out of bounds");
            return (m_array[index]);
        }

		T& front()
		{
            ASSERT_EQUAL(m_size != 0, true, "HeapArray Copy: Index out of bounds");
			return (m_array[0]);
		}

		T& back()
		{
           	ASSERT_EQUAL(m_size != 0, true, "HeapArray Copy: Index out of bounds");
			return (m_array[(m_size - 1)]);
		}

		void push_back(const T& value)
		{
			
			new (m_array + m_size++) T(value);
		}

        void pop_back()
        {
			ASSERT_EQUAL(m_size != 0, true, "HeapArray Copy: Index out of bounds");
			m_allocator.destroy(&m_array[(m_size-- - 1)]);
        }

		void emplace_back()
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T();
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1, arg2, arg3);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1, arg2, arg3);
        }	
        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, Arg3& arg3) // lol
        {
			ASSERT_EQUAL(m_array && m_size < m_capacity, true, "HeapArray Copy: Index out of bounds");
			new (m_array + m_size++) T(arg1, arg2, arg3);
        }	


		template <typename U>
		class ArrayIterator
		{
			public:
				typedef U                                   value_type;
				typedef U*                                  pointer;
				typedef U&                                  reference;
				typedef std::ptrdiff_t                      difference_type;
				typedef std::random_access_iterator_tag     iterator_category;

				ArrayIterator(pointer ptr) : m_ptr(ptr) {}
				ArrayIterator(const ArrayIterator& other) : m_ptr(other.m_ptr) {}
				~ArrayIterator() {}
				ArrayIterator& operator=(const ArrayIterator& other) { m_ptr = other.m_ptr; return *this; }


				reference operator*() const { return *m_ptr; }
				pointer operator->() const { return m_ptr; }

				ArrayIterator& operator++()
				{
					++m_ptr;
					return *this;
				}

				ArrayIterator operator++(int)
				{
					ArrayIterator tmp = *this;
					++m_ptr;
					return tmp;
				}

				ArrayIterator& operator--()
				{
					--m_ptr;
					return *this;
				}

				ArrayIterator operator--(int)
				{
					ArrayIterator tmp = *this;
					--m_ptr;
					return tmp;
				}

				bool operator==(const ArrayIterator& other) const { return m_ptr == other.m_ptr; }
				bool operator!=(const ArrayIterator& other) const { return m_ptr != other.m_ptr; }


				reference operator[](size_t index) const { return *(m_ptr + index); }
				ArrayIterator operator+(size_t n) const { return ArrayIterator(m_ptr + n); }
				ArrayIterator operator-(size_t n) const { return ArrayIterator(m_ptr - n); }
				difference_type operator-(const ArrayIterator& other) const { return m_ptr - other.m_ptr; }

				ArrayIterator& operator+=(size_t n)
				{
					m_ptr += n;
					return *this;
				}
				ArrayIterator& operator-=(size_t n)
				{
					m_ptr -= n;
					return *this;
				}

			private:
				pointer m_ptr;
		};

        iterator begin()						 	{ return (iterator				(&m_array[0])); }
        iterator end() 								{ return (iterator				(&m_array[m_size])); }

        const_iterator begin() const 				{ return (const_iterator		(&m_array[0])); }
        const_iterator end() const 					{ return (const_iterator		(&m_array[m_size])); }

        reverse_iterator rbegin() 					{ return (reverse_iterator		(&m_array[m_size - 1])); }
        reverse_iterator rend() 					{ return (reverse_iterator		(&m_array[-1])); }

        const_reverse_iterator rbegin() const 		{ return (const_reverse_iterator(&m_array[m_size - 1])); }
        const_reverse_iterator rend() const 		{ return (const_reverse_iterator(&m_array[-1])); }

	private:
		Allocator					m_allocator;
		T*							m_array;
		size_t						m_size;
		const size_t				m_capacity;
};


#endif

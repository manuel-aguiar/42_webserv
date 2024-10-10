/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:43:01 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 13:58:35 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TPP

# define LIST_TPP

template <typename T>
class Node
{
	public:
		Node(const T& data) : m_data(data), m_next(NULL), m_prev(NULL) {}

		Node() : m_data (), m_next(NULL), m_prev(NULL) {}

		template<typename Arg1>
		Node(const Arg1& arg1) : m_data(arg1), m_next(NULL), m_prev(NULL) {}

		template<typename Arg1, typename Arg2>
		Node(const Arg1& arg1, const Arg2& arg2) : m_data(arg1, arg2), m_next(NULL), m_prev(NULL) {}

		template<typename Arg1, typename Arg2, typename Arg3>
		Node(const Arg1& arg1, const Arg2& arg2, const Arg2& arg3) : m_data(arg1, arg2, arg3), m_next(NULL), m_prev(NULL) {}

		Node(const Node& other) : m_data(other.m_data), m_next(other.m_next), m_prev(other.m_prev) {}
		Node& operator=(const Node& other)
		{
			if (this == &other)
				return (*this);
			m_data = other.m_data;
			m_next = other.m_next;
			m_prev = other.m_prev;
			return (*this);
		}
		~Node()
		{

		}

		T 		m_data;
		Node* 	m_next;
		Node* 	m_prev;
};

template <typename T, typename Allocator>
class List
{
	typedef typename Allocator::template rebind<Node<T> >::other NodeAllocator;


	public:
		List(Allocator alloc = Allocator())
			: m_size(0), m_head(NULL), m_tail(NULL), m_nodeAllocator(NodeAllocator(alloc)) {}

		List(const List& other)
			: m_size(0), m_head(NULL), m_tail(NULL), m_nodeAllocator(other.m_nodeAllocator)
		{
			*this = other;
		}

		List& operator=(const List& other)
		{
			if (this == &other)
				return (*this);
			clear();
			Node<T>* current = other.m_head;
			while (current)
			{
				push_back(current->m_data);
				current = current->m_next;
			}
			return (*this);
		}

		~List()
		{
			clear();
		}

		size_t	size() const { return (m_size); }

		void	clear()
		{
			Node<T>* cur;
			Node<T>* next;

			cur = m_head;
			while (cur)
			{
				next = cur->m_next;
				m_nodeAllocator.destroy(cur);
				m_nodeAllocator.deallocate(cur, 1);
				cur = next;
			}
			m_size = 0;
			m_head = NULL;
			m_tail = NULL;
		}

		void	push_back(const T& data)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(data);
			_insertTail(node);
			++m_size;
		}

		void	push_front(const T& data)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(data);
			_insertHead(node);
			++m_size;
		}

		void	pop_back()
		{
			Node<T>* node;

			if (!m_tail)
				return ;
			node = m_tail;
			m_tail = m_tail->m_prev;
			if (m_tail)
				m_tail->m_next = NULL;
			else
				m_head = NULL;
			m_nodeAllocator.destroy(node);
			m_nodeAllocator.deallocate(node, 1);
			--m_size;
		}

		void	pop_front()
		{
			Node<T>* node;

			if (!m_head)
				return ;
			node = m_head;
			m_head = m_head->m_next;
			if (m_head)
				m_head->m_prev = NULL;
			else
				m_tail = NULL;
			m_nodeAllocator.destroy(node);
			m_nodeAllocator.deallocate(node, 1);
			--m_size;
		}

		void 	emplace_back()
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>();
			_insertTail(node);
			++m_size;
		}

		template <typename Arg1 >
		void	emplace_back(const Arg1& arg1)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1);
			_insertTail(node);
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2);
			_insertTail(node);
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2, arg3);
			_insertTail(node);
			++m_size;
		}

		void 	emplace_front()
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>();
			_insertHead(node);
			++m_size;
		}

		template <typename Arg1 >
		void	emplace_front(const Arg1& arg1)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1);
			_insertHead(node);
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2);
			_insertHead(node);
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2, arg3);
			_insertHead(node);
			++m_size;
		}


        class iterator
        {
            public:
                typedef Node<T>                                   value_type;
                typedef Node<T>*                                  pointer;
                typedef Node<T>&                                  reference;

                iterator(pointer ptr) : m_ptr(ptr) {}
                iterator(const iterator& other) : m_ptr(other.m_ptr) {}
                ~iterator() {}


                T& operator*() const { return m_ptr->m_data; }
                T* operator->() const { return &m_ptr->m_data; }

				bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
				bool operator!=(const iterator& other) const { return m_ptr != other.m_ptr; }

                iterator& operator++()
                {
                    m_ptr = m_ptr->m_next;
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator tmp = *this;
                    m_ptr = m_ptr->m_next;
                    return tmp;
                }

                iterator& operator--()
                {
                    m_ptr = m_ptr->m_prev;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator tmp = *this;
                    m_ptr = m_ptr->m_prev;
                    return tmp;
                }
            private:
                pointer m_ptr;
        };

		iterator begin() { return iterator(m_head); }
		iterator end() { return iterator(NULL); }

	private:
		size_t					m_size;
		Node<T>* 				m_head;
		Node<T>* 				m_tail;
		NodeAllocator			m_nodeAllocator;

		void 	_insertTail(Node<T>* node)
		{
			if (!m_head)
			{
				m_head = node;
				m_tail = node;
			}
			else
			{
				m_tail->m_next = node;
				node->m_prev = m_tail;
				m_tail = node;
			}
		}

		void	_insertHead(Node<T>* node)
		{
			if (!m_head)
			{
				m_head = node;
				m_tail = node;
			}
			else
			{
				m_head->m_prev = node;
				node->m_next = m_head;
				m_head = node;
			}
		}



};



#endif

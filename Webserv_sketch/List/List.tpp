/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:43:01 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 11:55:38 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TPP

# define LIST_TPP

template <typename T>
class Node
{
	public:
		Node(const T& data) : data(data), next(NULL), prev(NULL) {}

		Node() : data (), next(NULL), prev(NULL) {}

		template<typename Arg1>
		Node(const Arg1& arg1) : data(arg1), next(NULL), prev(NULL) {}

		template<typename Arg1, typename Arg2>
		Node(const Arg1& arg1, const Arg2& arg2) : data(arg1, arg2), next(NULL), prev(NULL) {}

		template<typename Arg1, typename Arg2, typename Arg3>
		Node(const Arg1& arg1, const Arg2& arg2, const Arg2& arg3) : data(arg1, arg2, arg3), next(NULL), prev(NULL) {}

		Node(const Node& other) : data(other.data), next(other.next), prev(other.prev) {}
		Node& operator=(const Node& other)
		{
			if (this == &other)
				return (*this);
			data = other.data;
			next = other.next;
			prev = other.prev;
			return (*this);
		}
		~Node()
		{

		}

		T data;
		Node* next;
		Node* prev;
};

template <typename T, typename Allocator>
class List
{
	typedef typename Allocator::template rebind<Node<T> >::other NodeAllocator;


	public:
		List(Allocator alloc = Allocator())
			: m_head(NULL), m_tail(NULL), m_nodeAllocator(NodeAllocator(alloc)) {}

		List(const List& other)
			: m_head(NULL), m_tail(NULL), m_nodeAllocator(other.m_nodeAllocator)
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
				push_back(current->data);
				current = current->next;
			}
			return (*this);
		}

		~List()
		{
			clear();
		}

		void	clear()
		{
			Node<T>* cur;
			Node<T>* next;

			cur = m_head;
			while (cur)
			{
				next = cur->next;
				m_nodeAllocator.destroy(cur);
				m_nodeAllocator.deallocate(cur, 1);
				cur = next;
			}
			m_head = NULL;
			m_tail = NULL;
		}

		void	push_back(const T& data)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(data);
			_insertTail(node);
		}

		void	push_front(const T& data)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(data);
			_insertHead(node);
		}

		void	pop_back()
		{
			Node<T>* node;

			if (!m_tail)
				return ;
			node = m_tail;
			m_tail = m_tail->prev;
			if (m_tail)
				m_tail->next = NULL;
			else
				m_head = NULL;
			m_nodeAllocator.destroy(node);
			m_nodeAllocator.deallocate(node, 1);
		}

		void	pop_front()
		{
			Node<T>* node;

			if (!m_head)
				return ;
			node = m_head;
			m_head = m_head->next;
			if (m_head)
				m_head->prev = NULL;
			else
				m_tail = NULL;
			m_nodeAllocator.destroy(node);
			m_nodeAllocator.deallocate(node, 1);
		}

		void 	emplace_back()
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>();
			_insertTail(node);
		}

		template <typename Arg1 >
		void	emplace_back(const Arg1& arg1)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1);
			_insertTail(node);
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2);
			_insertTail(node);
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2, arg3);
			_insertTail(node);
		}

		void 	emplace_front()
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>();
			_insertHead(node);
		}

		template <typename Arg1 >
		void	emplace_front(const Arg1& arg1)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1);
			_insertHead(node);
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2);
			_insertHead(node);
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			Node<T>* node = m_nodeAllocator.allocate(1);
			new (node) Node<T>(arg1, arg2, arg3);
			_insertHead(node);
		}

	private:

		void 	_insertTail(Node<T>* node)
		{
			if (!m_head)
			{
				m_head = node;
				m_tail = node;
			}
			else
			{
				m_tail->next = node;
				node->prev = m_tail;
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
				m_head->prev = node;
				node->next = m_head;
				m_head = node;
			}
		}

		Node<T>* 				m_head;
		Node<T>* 				m_tail;
		NodeAllocator			m_nodeAllocator;
};



#endif

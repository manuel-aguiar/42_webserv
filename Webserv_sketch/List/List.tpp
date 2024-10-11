/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:43:01 by manuel            #+#    #+#             */
/*   Updated: 2024/10/11 10:14:33 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TPP

# define LIST_TPP



template <typename T, typename Allocator>
class List
{

	private:

		struct BaseNode
		{
			BaseNode() : m_next(NULL), m_prev(NULL) {}
			BaseNode(BaseNode* next, BaseNode* prev) : m_next(next), m_prev(prev) {}
			BaseNode(const BaseNode& other) : m_next(other.m_next), m_prev(other.m_prev) {}
			BaseNode& operator=(const BaseNode& other)
			{
				if (this == &other)
					return (*this);
				m_next = other.m_next;
				m_prev = other.m_prev;
				return (*this);
			}
			~BaseNode() {}

			BaseNode* m_next;
			BaseNode* m_prev;
		};

		struct HeaderNode : public BaseNode
		{
			HeaderNode() : BaseNode(static_cast<BaseNode*>(this), static_cast<BaseNode*>(this)) {}
			HeaderNode(const HeaderNode& other) : BaseNode(other) {}
			HeaderNode& operator=(const HeaderNode& other)
			{
				if (this == &other)
					return (*this);
				BaseNode::operator=(other);
				return (*this);
			}
			~HeaderNode() {}
		};

		struct DataNode : public BaseNode
		{
			DataNode(const T& data) : BaseNode(), m_data(data) {}

			DataNode() : BaseNode(), m_data () {}

			template<typename Arg1>
			DataNode(const Arg1& arg1) : BaseNode(), m_data (arg1) {}

			template<typename Arg1, typename Arg2>
			DataNode(const Arg1& arg1, const Arg2& arg2) : BaseNode(), m_data (arg1, arg2) {}

			template<typename Arg1, typename Arg2, typename Arg3>
			DataNode(const Arg1& arg1, const Arg2& arg2, const Arg2& arg3) : BaseNode(), m_data(arg1, arg2, arg3) {}

			DataNode(const DataNode& other) : BaseNode(other), m_data(other.m_data) {}
			DataNode& operator=(const DataNode& other)
			{
				if (this == &other)
					return (*this);
				BaseNode::operator=(other);
				m_data = other.m_data;
				return (*this);
			}
			~DataNode() {}

			T 		m_data;
		};

	typedef typename Allocator::template rebind<DataNode>::other NodeAllocator;

	public:

		List(Allocator alloc = Allocator())
			: m_size(0), m_header(), m_nodeAllocator(NodeAllocator(alloc)) {}

		List(const List& other)
			: m_size(0), m_header(), m_nodeAllocator(other.m_nodeAllocator)
		{
			*this = other;
		}

		List& operator=(const List& other)
		{
			if (this == &other)
				return (*this);
			clear();
			BaseNode* current = other.m_header.m_next;
			while (current != &other.m_header)
			{
				emplace_back(current->m_data);
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
			BaseNode* cur;
			BaseNode* next;
			DataNode* data;

			cur = m_header.m_next;
			while (cur != &m_header)
			{
				next = cur->m_next;
				data = static_cast<DataNode*>(cur);
				m_nodeAllocator.destroy(data);
				m_nodeAllocator.deallocate(data, 1);
				cur = next;
			}
			m_size = 0;
			m_header.m_next = &m_header;
			m_header.m_prev = &m_header;
		}

		void	push_back(const T& data)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(data);
			_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		void	push_front(const T& data)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(data);
			_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		void	pop_back()
		{
			BaseNode* node;
			DataNode* data;

			node = m_header.m_prev;
			_removeTarget(m_header.m_prev);

			data = static_cast<DataNode*>(node);
			m_nodeAllocator.destroy(data);
			m_nodeAllocator.deallocate(data, 1);
			--m_size;
		}

		void	pop_front()
		{
			BaseNode* node;
			DataNode* data;

			node = m_header.m_next;
			_removeTarget(m_header.m_next);

			data = static_cast<DataNode*>(node);
			m_nodeAllocator.destroy(data);
			m_nodeAllocator.deallocate(data, 1);
			--m_size;
		}

		void 	emplace_back()
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode();
			_insertAfter(node);
			++m_size;
		}

		template <typename Arg1 >
		void	emplace_back(const Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		void 	emplace_front()
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode();
			_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1 >
		void	emplace_front(const Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}


        class iterator
        {
            public:
                typedef BaseNode                                   value_type;
                typedef BaseNode*                                  pointer;
                typedef BaseNode&                                  reference;

                iterator(pointer ptr) : m_ptr(ptr) {}
                iterator(const iterator& other) : m_ptr(other.m_ptr) {}
                ~iterator() {}


                T& operator*() const { return static_cast<DataNode*>(m_ptr)->m_data; }
                T* operator->() const { return &static_cast<DataNode*>(m_ptr)->m_data; }

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

		iterator begin() { return iterator(m_header.m_next); }
		iterator end() { return iterator(&m_header); }

	private:
		size_t					m_size;
		HeaderNode 				m_header;
		NodeAllocator			m_nodeAllocator;

		void 	_insertAfter(BaseNode* target, BaseNode* node)
		{
			node->m_prev = target;
			node->m_next = target->m_next;
			node->m_prev->m_next = node;
			node->m_next->m_prev = node;
		}

		void	_insertBefore(BaseNode* target, BaseNode* node)
		{
			node->m_next = target;
			node->m_prev = target->m_prev;
			node->m_prev->m_next = node;
			node->m_next->m_prev = node;
		}

		void	_removeTarget(BaseNode* target)
		{
			target->m_next->m_prev = target->m_prev;
			target->m_prev->m_next = target->m_next;
		}

};



#endif

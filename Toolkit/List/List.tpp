/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:43:01 by manuel            #+#    #+#             */
/*   Updated: 2024/12/17 12:52:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_TPP

# define LIST_TPP

#include <cassert>

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

		List(const NodeAllocator& alloc = Allocator())
			: m_size(0), m_header(), m_nodeAllocator(alloc)
		{
			
		}


		List(const List& other)
			: m_size(0), m_header(), m_nodeAllocator(other.m_nodeAllocator)
		{
			*this = other;
		}

		List& operator=(const List& other)
		{
			if (this == &other)
				return (*this);

			BaseNode* thisCur = m_header.m_next;
			BaseNode* otherCur = other.m_header.m_next;

			while (thisCur != &m_header && otherCur != &other.m_header)
			{
				static_cast<DataNode*>(thisCur)->m_data = static_cast<DataNode*>(otherCur)->m_data;
				otherCur = otherCur->m_next;
				thisCur = thisCur->m_next;
			}
			while (thisCur != &m_header)
			{
				BaseNode* next = thisCur->m_next;
				mf_removeTarget(thisCur);
				mf_deallocate(thisCur);
				thisCur = next;
				--m_size;
			}
			while (otherCur != &other.m_header)
			{
				push_back(static_cast<DataNode*>(otherCur)->m_data);
				otherCur = otherCur->m_next;
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

			cur = m_header.m_next;
			while (cur != &m_header)
			{
				next = cur->m_next;
				mf_deallocate(cur);
				cur = next;
			}
			m_size = 0;
			m_header.m_next = &m_header;
			m_header.m_prev = &m_header;
		}

		T&		front() { assert(m_size > 0); return static_cast<DataNode*>(m_header.m_next)->m_data; }
		T&		back() { assert(m_size > 0); return static_cast<DataNode*>(m_header.m_prev)->m_data; }

		void	push_back(const T& data)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(data);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		void	push_front(const T& data)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			
			new (node) DataNode(data);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}



		void	pop_back()
		{
			BaseNode* node;

			node = m_header.m_prev;
			mf_removeTarget(m_header.m_prev);
			mf_deallocate(node);
			--m_size;
		}

		void	pop_front()
		{
			BaseNode* node;

			node = m_header.m_next;
			mf_removeTarget(m_header.m_next);
			mf_deallocate(node);
			--m_size;
		}
		
		void 	emplace_back()
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode();
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1>
		void	emplace_back(Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_back(Arg1& arg1, Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}


		template <typename Arg1 >
		void	emplace_back(const Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			mf_insertAfter(m_header.m_prev, static_cast<BaseNode*>(node));
			++m_size;
		}

		void 	emplace_front()
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode();
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1 >
		void	emplace_front(Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_front(Arg1& arg1, Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_front(Arg1& arg1, Arg2& arg2, Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}


		template <typename Arg1 >
		void	emplace_front(const Arg1& arg1)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
			++m_size;
		}

		template <typename Arg1, typename Arg2, typename Arg3 >
		void	emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			DataNode* node = m_nodeAllocator.allocate(1);
			new (node) DataNode(arg1, arg2, arg3);
			mf_insertBefore(m_header.m_next, static_cast<BaseNode*>(node));
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

				pointer getPtr () const { return m_ptr; }
            private:
                pointer m_ptr;
        };

		iterator begin() { return iterator(m_header.m_next); }
		iterator end() { return iterator(&m_header); }

		void	splice(iterator pos, List& other)
		{
			BaseNode* target = pos.getPtr();
			BaseNode* otherCur;

			assert(mf_iterIsInList(target));
			if (other.m_size == 0)
				return ;

			otherCur = other.m_header.m_next;
			target->m_prev->m_next = otherCur;
			otherCur->m_prev = target->m_prev;

			otherCur = other.m_header.m_prev;
			target->m_prev = otherCur;
			otherCur->m_next = target;

			m_size += other.m_size;
			other.m_size = 0;
			other.m_header.m_next = &other.m_header;
			other.m_header.m_prev = &other.m_header;
		}

		void	splice(iterator pos, List& other, iterator itOther)
		{
			BaseNode* thisTgt = pos.getPtr();
			BaseNode* otherTgt = itOther.getPtr();

			assert(mf_iterIsInList(thisTgt));
			assert(other.mf_iterIsInList(otherTgt));

			if (other.m_size == 0)
				return ;

			other.mf_removeTarget(otherTgt);
			this->mf_insertBefore(thisTgt, otherTgt);

			m_size += 1;
			other.m_size -= 1;
		}

		void	splice(iterator pos, List& other, iterator otherStart, iterator otherEnd)
		{
			BaseNode* thisTgt = pos.getPtr();
			BaseNode* start = otherStart.getPtr();
			BaseNode* end = otherEnd.getPtr();

			assert(mf_iterIsInList(thisTgt));
			assert(other.mf_iterIsInList(start));
			assert(other.mf_iterIsInList(end));
			assert(other.mf_canReach(start, end));

			if (other.m_size == 0)
				return ;

			while (start != end)
			{
				BaseNode* next = start->m_next;
				other.mf_removeTarget(start);
				this->mf_insertBefore(thisTgt, start);
				start = next;
				++m_size;
				--other.m_size;
			}
		}

	private:
		size_t					m_size;
		HeaderNode 				m_header;
		NodeAllocator			m_nodeAllocator;

		void 	mf_insertAfter(BaseNode* target, BaseNode* node)
		{
			node->m_prev = target;
			node->m_next = target->m_next;
			node->m_prev->m_next = node;
			node->m_next->m_prev = node;
		}

		void	mf_insertBefore(BaseNode* target, BaseNode* node)
		{
			node->m_next = target;
			node->m_prev = target->m_prev;
			node->m_prev->m_next = node;
			node->m_next->m_prev = node;
		}

		void	mf_removeTarget(BaseNode* target)
		{
			target->m_next->m_prev = target->m_prev;
			target->m_prev->m_next = target->m_next;
		}

		void	mf_deallocate(BaseNode* node)
		{
			DataNode* data = static_cast<DataNode*>(node);
			m_nodeAllocator.destroy(data);
			m_nodeAllocator.deallocate(data, 1);
		}

		bool	mf_iterIsInList(BaseNode* node)
		{
			BaseNode* cur = m_header.m_next;

			while (cur != &m_header)
			{
				if (cur == node)
					return (true);
				cur = cur->m_next;
			}
			if (cur == node)
				return (true);
			return (false);
		}

		bool	mf_canReach(BaseNode* start, BaseNode* end)
		{
			while (start != &m_header)
			{
				if (start == end)
					return (true);
				start = start->m_next;
			}
			if (start == end)
				return (true);
			return (false);
		}

};



#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.tpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:33:51 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 17:18:53 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_TPP

# define MAP_TPP




template <
	typename Key,
	typename Value,
	typename Compare,
	typename Allocator
>
class Map
{

	private:

		struct Pair
		{
			Pair(const Key& key, Value& value) : m_key(key), m_value(value) {}

			const Key 	first;
			Value		second;
		};

		struct Node
		{
			Node(const Pair& pair) : m_pair(pair), m_left(NULL), m_right(NULL), m_parent(NULL), m_red(true) {}

			Node* 		m_left;
			Node* 		m_right;
			Node* 		m_parent;
			Pair		m_pair;
			bool		m_red;
		};

	typedef typename Allocator::template rebind<Pair>::other NodeAllocator;

	public:
		Map(Compare compare = Compare(), Allocator allocator = Allocator()) :
			m_root(NULL), m_size(0), m_compare(compare), m_nodeAllocator(NodeAllocator(allocator)) {}
		Map(const Map& other) : m_compare(other.m_compare), m_nodeAllocator(other.m_nodeAllocator)
		{
			*this = other;
		}
		~Map()
		{

		}
		Map& operator=(const Map& other)
		{
			if (this == &other)
				return (*this);
			return (*this);
		}

		void insert(const Key& key, const Value& value)
		{

		}

		void	clear()
		{

		}

		size_t	size() const
		{
			return (m_size);
		}

	private:
		Node*			m_root;
		size_t			m_size;
		Compare			m_compare;
		NodeAllocator	m_nodeAllocator;

		Node*	mf_insert(Node* node, const Key& key, const Value& value)
		{
			if (!node)
			{
				node = m_nodeAllocator.allocate(1);
				m_nodeAllocator.construct(node, Pair(key, value));
				return (node);
			}
			if (m_compare(key, node->m_pair.first))
			{
				node->m_left = mf_insert(node->m_left, key, value);
				node->m_left->m_parent = node;
			}
			else if (m_compare(node->m_pair.first, key))
			{
				node->m_right = mf_insert(node->m_right, key, value);
				node->m_right->m_parent = node;
			}
			else
				node->m_pair.second = value;
			return (node);
		}

};




#endif

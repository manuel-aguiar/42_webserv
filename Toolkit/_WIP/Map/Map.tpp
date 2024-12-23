/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.tpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:33:51 by manuel            #+#    #+#             */
/*   Updated: 2024/12/23 10:13:56 by mmaria-d         ###   ########.fr       */
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

		struct BaseNode
		{
			BaseNode() : m_left(NULL), m_right(NULL), m_parent(NULL), m_red(false) {}
			BaseNode(BaseNode* left, BaseNode* right, BaseNode* parent, bool isRed)
				: m_left(left), m_right(right), m_parent(parent), m_red(isRed) {}

			BaseNode(const BaseNode& other)
				: m_left(other.m_left), m_right(other.m_right), m_parent(other.m_parent), m_red(other.m_red) {}

			BaseNode& operator=(const BaseNode& other)
			{
				if (this == &other)
					return (*this);
				m_left = other.m_left;
				m_right = other.m_right;
				m_parent = other.m_parent;
				m_red = other.m_red;
				return (*this);
			}

			BaseNode* 	m_left;
			BaseNode* 	m_right;
			BaseNode* 	m_parent;
			bool		m_red;
		}

		struct HeaderNode : public BaseNode
		{
			HeaderNode() : BaseNode(static_cast<BaseNode*>(this),
									static_cast<BaseNode*>(this),
									static_cast<BaseNode*>(this),
									false) {}

			HeaderNode(const HeaderNode& other) : BaseNode(other) {}
			HeaderNode& operator=(const HeaderNode& other)
			{
				if (this == &other)
					return (*this);
				BaseNode::operator=(other);
				return (*this);
			}
		};


		struct DataNode : public BaseNode
		{
			DataNode(const Key& key, const Value& value) : BaseNode(), m_pair(key, value) {}

			DataNode& operator=(const DataNode& other)
			{
				if (this == &other)
					return (*this);
				BaseNode::operator=(other);
				m_data = other.m_data;
				return (*this);
			}
			~DataNode() {}

			Pair 		m_pair;
		};


		struct Pair
		{
			Pair(const Key& key, Value& value) : m_key(key), m_value(value) {}

			const Key 	first;
			Value		second;
		};

	typedef typename Allocator::template rebind<Pair>::other NodeAllocator;

	public:
		Map(Compare compare = Compare(), const NodeAllocator& allocator = NodeAllocator()) :
			m_header(), m_root(static_cast<BaseNode*>(&m_header)),
			m_size(0), m_compare(compare),
			m_nodeAllocator(allocator) {}
		Map(const Map& other) : m_nodeAllocator(other.m_nodeAllocator)
							{*this = other;}			//later
		~Map() {}										//later
		Map& operator=(const Map& other)
		{
			if (this == &other)
				return (*this);
			return (*this);
		}

		void insert(const Key& key, const Value& value)
		{
			if (m_size == 0) {
				// Create a new DataNode as the root
				DataNode* newNode = m_nodeAllocator.allocate(1);
				new (&newNode->m_data) Pair(key, value); // Construct Pair in the DataNode
				m_root = newNode; // Set the new node as the root
				m_root->m_parent = &m_header; // Link to the header as parent (sentinel)
				m_header.m_left = m_root; // Link the header node
				m_header.m_right = m_root; // For a simple case, can be adjusted as needed
				++m_size;
			} else {
				// Insert into the tree using the recursive helper
				m_root = mf_insert(m_root, key, value);
    		}
		}

		void	clear()
		{

		}

		size_t	size() const
		{
			return (m_size);
		}

	private:
		HeaderNode		m_header;
		BaseNode*		m_root;
		size_t			m_size;
		Compare			m_compare;
		NodeAllocator	m_nodeAllocator;

		BaseNode*	mf_insert(BaseNode* node, const Key& key, const Value& value)
		{
			DataNode* data;

			if (!node)
			{
				data = m_nodeAllocator.allocate(1);
				new (data) DataNode(key, value);
				return (static_cast<BaseNode*>(data));
			}
			if (m_compare(key, static_cast<DataNode*>(node)->m_pair.first))
			{
				node->m_left = mf_insert(node->m_left, key, value);
				node->m_left->m_parent = node;
			}
			else if (m_compare(static_cast<DataNode*>(node)->m_pair.first, key))
			{
				node->m_right = mf_insert(node->m_right, key, value);
				node->m_right->m_parent = node;
			}
			else
				static_cast<DataNode*>(node)->m_pair.second = value;
			return (node);
		}

};




#endif

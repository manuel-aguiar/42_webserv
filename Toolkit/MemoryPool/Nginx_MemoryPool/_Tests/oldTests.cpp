/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxPoolAllocTests.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:00:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:01:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <map>
# include <iostream>
# include <vector>
# include <string>
# include "../MemoryPool.h"


class ToolkitDummy
{
	public:
		ToolkitDummy() : m_data(new int [4])
		{
			std::cout << "constructor" << std::endl;
		};
		~ToolkitDummy()
		{
			std::cout << "dummy destroy" << std::endl; 
			if (m_data)  delete [] m_data; 
				m_data = NULL;
		};
		ToolkitDummy(const ToolkitDummy& other) : m_data(new int [4])
		{
			std::cout << "dummy copy" << std::endl; 
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
		};
		ToolkitDummy& operator=(const ToolkitDummy& other)
		{
			if (!m_data)
				m_data = new int [4];
			std::cout << " copy assignment" << std::endl;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int)); 
			return (*this);
		};

	private:
		int* m_data;
};


int main(void)
{
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

    Nginx_PoolAllocator<char> alloc(pool);
	StringInPool *str = (StringInPool*)pool->allocate(sizeof(StringInPool), sizeof(StringInPool));
	new (str) StringInPool("Hello, Memory Pool!", alloc);
    str->reserve(100);

	Nginx_PoolAllocator<int> alloc2(pool);
	VectorInPool<int>::type*  vec = (VectorInPool<int>::type*)pool->allocate(sizeof(VectorInPool<int>::type), sizeof(VectorInPool<int>::type));
	new (vec) VectorInPool<int>::type(alloc2);	

	vec->reserve(5);
	vec->push_back(1);
	vec->push_back(2);
	vec->push_back(3);
	vec->push_back(4);
	vec->push_back(5);
	vec->push_back(6);
	vec->push_back(7);

	Nginx_PoolAllocator<std::pair<int, int> > alloc3(pool);

	MapInPool<int, int>::type* myMap = (MapInPool<int, int>::type*)pool->allocate(sizeof(MapInPool<int, int>::type), sizeof(MapInPool<int, int>::type));
	new (myMap) MapInPool<int, int>::type(std::less<int>(), alloc3);
	
	myMap->insert(std::make_pair(2, 100));
	myMap->insert(std::make_pair(3, 100));
	myMap->insert(std::make_pair(4, 100));
	myMap->insert(std::make_pair(5, 100));
	myMap->insert(std::make_pair(6, 100));
	myMap->insert(std::make_pair(7, 100));
	myMap->insert(std::make_pair(8, 100));
	myMap->insert(std::make_pair(9, 100));
	myMap->insert(std::make_pair(10, 100));
	myMap->insert(std::make_pair(11, 100));
	myMap->insert(std::make_pair(12, 100));
	myMap->insert(std::make_pair(13, 100));
	myMap->insert(std::make_pair(14, 100));


	

	for (MapInPool<int, int>::type::iterator it = myMap->begin(); it != myMap->end(); ++it)
	{
		std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
	}

	vec->push_back(8);
	vec->push_back(9);
	vec->push_back(123);

	Nginx_PoolAllocator<std::pair<int, ToolkitDummy> > alloc4(pool);

	MapInPool<int, ToolkitDummy>::type* myMap2 = (MapInPool<int, ToolkitDummy>::type*)pool->allocate(sizeof(MapInPool<int, ToolkitDummy>::type), sizeof(MapInPool<int, ToolkitDummy>::type));
	new (myMap2) MapInPool<int, ToolkitDummy>::type(std::less<int>(), alloc4);

	myMap2->insert(std::make_pair(2, ToolkitDummy()));
	myMap2->insert(std::make_pair(3, ToolkitDummy()));

	myMap2->insert(std::make_pair(3, ToolkitDummy()));
	myMap2->insert(std::make_pair(4, ToolkitDummy()));
	myMap2->insert(std::make_pair(5, ToolkitDummy()));
	myMap2->insert(std::make_pair(6, ToolkitDummy()));

	(*myMap2)[15] = ToolkitDummy();

	myMap->~map();
	myMap2->~map();


	Nginx_PoolAllocator<ToolkitDummy> alloc5(pool);
	VectorInPool<ToolkitDummy>::type*  vec2 = (VectorInPool<ToolkitDummy>::type*)pool->allocate(sizeof(VectorInPool<ToolkitDummy>::type), sizeof(VectorInPool<ToolkitDummy>::type));
	new (vec2) VectorInPool<ToolkitDummy>::type(alloc5);	

	vec2->reserve(7);

	vec2->push_back(ToolkitDummy());
	vec2->push_back(ToolkitDummy());


	// this is the poor man's std::vector::emplace_back()
	// in reality, with emplace back, the vector "knows" that the object is constructed in place
	// and adjusts size(). Our way.. it doesn't. and we dont have setters for size
	// which means this can go wrong pretty fast
	// fun experiment, though

	new (&((*vec2)[2])) ToolkitDummy();
	new (&((*vec2)[3])) ToolkitDummy();
	new (&((*vec2)[4])) ToolkitDummy();
	new (&((*vec2)[5])) ToolkitDummy();
	new (&((*vec2)[6])) ToolkitDummy();


	((*vec2)[2]).~ToolkitDummy();
	((*vec2)[3]).~ToolkitDummy();
	((*vec2)[4]).~ToolkitDummy();
	((*vec2)[5]).~ToolkitDummy();
	((*vec2)[6]).~ToolkitDummy();

	vec2->~vector();

	pool->destroy();

	return (0);
}
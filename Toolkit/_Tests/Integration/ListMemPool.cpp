/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListMemPool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 11:27:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 11:31:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ListMemPool(int testNumber)
{
    
/******************* *** ************************/

	Nginx_MemoryPool* memoryPool = Nginx_MemoryPool::create(4096);

	

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<ToolkitDummy> alloc(memoryPool);
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > std(alloc);
		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = list.begin();
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/********************************************* */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> > std(allocString);
		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("super long string that happens to be bigger than internal buffer", allocChar));
			list.emplace_back("super long string that happens to be bigger than internal buffer", allocChar);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> > std(allocString);
		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool("cenas e coisas", allocChar));

			std.push_back(StringInPool(" coisas", allocChar));
			list.push_back(StringInPool(" coisas", allocChar));

			std.pop_front();
			list.pop_front();

			std.push_front(StringInPool(" tretas coisas", allocChar));
			list.push_front(StringInPool(" tretas coisas", allocChar));

			std.pop_back();
			list.pop_back();

			std.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));

			std.push_back(StringInPool(" tretas coisas", allocChar));
			list.push_back(StringInPool(" tretas coisas", allocChar));

			std.pop_front();
			list.pop_front();


		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
    }


/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool(" coisas", allocChar));
			list.pop_front();
			list.push_front(StringInPool(" tretas coisas", allocChar));
			list.pop_back();
			list.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.push_back(StringInPool(" tretas coisas", allocChar));
			list.pop_front();
		}

		List<StringInPool, Nginx_PoolAllocator<StringInPool> > copy(list);

		if (copy.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = copy.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays

		std::list<StringInPool, Nginx_MPool_FixedElem<StringInPool> > 	std((Nginx_MPool_FixedElem<StringInPool>(memoryPool, 5000)));
		List<StringInPool, Nginx_MPool_FixedElem<StringInPool> > 		list((Nginx_MPool_FixedElem<StringInPool>(memoryPool, 5000)));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.emplace_back(StringInPool("cenas e coisas", allocChar));

			std.push_back(StringInPool(" coisas", allocChar));
			list.emplace_back(StringInPool(" coisas", allocChar));

			std.pop_front();
			list.pop_front();

			std.push_front(StringInPool(" tretas coisas", allocChar));
			list.emplace_front(StringInPool(" tretas coisas", allocChar));

			std.pop_back();
			list.pop_back();

			std.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.emplace_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));

			std.push_back(StringInPool(" tretas coisas", allocChar));
			list.emplace_back(StringInPool(" tretas coisas", allocChar));

			std.pop_front();
			list.pop_front();


		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<StringInPool, Nginx_MPool_FixedElem<StringInPool> >::iterator iter = std.begin();
		List<StringInPool, Nginx_MPool_FixedElem<StringInPool> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays


		MPool_FixedElem<StringInPool> alloc(200);
		std::list<StringInPool, MPool_FixedElem<StringInPool> > std(alloc);
		List<StringInPool, MPool_FixedElem<StringInPool> > 		list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool("cenas e coisas", allocChar));
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<StringInPool, MPool_FixedElem<StringInPool> >::iterator iter = std.begin();
		List<StringInPool, MPool_FixedElem<StringInPool> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		std::list<int, Nginx_MPool_FixedElem<int> > 	std(Nginx_MPool_FixedElem<int>(memoryPool, 100));
		List<int, Nginx_MPool_FixedElem<int> > 			list(Nginx_MPool_FixedElem<int>(memoryPool, 100));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<int, Nginx_MPool_FixedElem<int> >::iterator iter = std.begin();
		List<int, Nginx_MPool_FixedElem<int> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		std::list<int, Nginx_MPool_FixedElem<int> > 	std(Nginx_MPool_FixedElem<int>(memoryPool, 100));
		List<int, Nginx_MPool_FixedElem<int> > 			list(Nginx_MPool_FixedElem<int>(memoryPool, 100));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<int, Nginx_MPool_FixedElem<int> >::iterator iter = std.begin();
		List<int, Nginx_MPool_FixedElem<int> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<int> fixedElem(memoryPool, 300);
		Nginx_PoolAllocator_FixedElem<int> alloc(fixedElem);
		
		List<int, Nginx_PoolAllocator_FixedElem<int> > first(alloc);
		List<int, Nginx_PoolAllocator_FixedElem<int> > second(alloc);


		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}

		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator it = second.begin();
		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}


		// allocates on the same memory pool
		List<int, Nginx_PoolAllocator_FixedElem<int> > copy(second);

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("allocator mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/****************************************************** */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<int> fixedElem(memoryPool, 300);
		Nginx_PoolAllocator_FixedElem<int> alloc(fixedElem);
		
		List<int, Nginx_PoolAllocator_FixedElem<int> > first(alloc);
		List<int, Nginx_PoolAllocator_FixedElem<int> > second(alloc);


		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}

		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator it = second.begin();
		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}


		
		//Copy starts by alllocating somewhere else
		Nginx_MPool_FixedElem<int> fixedElem2(memoryPool, 100);
		Nginx_PoolAllocator_FixedElem<int> alloc2(fixedElem2);

		
		List<int, Nginx_PoolAllocator_FixedElem<int> > copy(alloc2);

		for (size_t i = 0; i < second.size() / 2; ++i)
			copy.emplace_back(i);

		// copy should allocate on the same memory pool as first
		copy = first;

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy assignment, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy assignment, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("copy assignment, allocator mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/********************************************************* */

/******************************************************** */	
	

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<ToolkitDummy> alloc(memoryPool);
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > std(alloc);
		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = list.begin();
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (&(*it) == &(*iter))
				throw std::logic_error("copy constructor, same pointer");
		}

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > copy(list);

		it = copy.begin();
		iter = std.begin();
		for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (&(*it) == &(*iter))
				throw std::logic_error("copy constructor, same pointer");
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************************************************** */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<ToolkitDummy> fixedElem(memoryPool, 200);

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > first(fixedElem);
		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > second(fixedElem);

		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}
		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> >::iterator it = second.begin();
		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > copy(second); /// should allocate on the same memory pool

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("allocator mismatch");

		std::cout << "	PASSED" << std::endl;
	}

	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}


	memoryPool->destroy();

    return (testNumber);
}

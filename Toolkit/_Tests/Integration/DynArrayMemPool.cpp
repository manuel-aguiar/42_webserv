/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DynArrayMemPool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:57:58 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 11:02:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int DynArrayMemPool(int testNumber)
{

	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

	/******************* TEST 4 ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Nginx_PoolAllocator<ToolkitDummy> alloc(pool);
		std::vector<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > 		std(alloc);
		DynArray<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > 			array(0, alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = array.begin();
		std::vector<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
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

/******************* TEST 5 ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Nginx_PoolAllocator<ToolkitBase*> alloc(pool);
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> > 		std(alloc);
		DynArray<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			delete std.back();
			delete array.back();

			std.pop_back();
			array.pop_back();
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> >::iterator it = array.begin();
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch");
		}

		it = array.begin();
		iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}


/*******************  ************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		Nginx_PoolAllocator<ToolkitBase*> alloc(pool);
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> > 		std(alloc);
		DynArray<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);
		ToolkitBase* base;

		for (int i = 0; i < 100; ++i)
		{
						base = (ToolkitBase *)pool->allocate(sizeof(ToolkitBase));
						new (base) ToolkitBase(i);
			std.push_back(base);

						base = (ToolkitBase *)pool->allocate(sizeof(ToolkitBase));
						new (base) ToolkitBase(i);

			array.emplace_back(base);

						base = (ToolkitDerived *)pool->allocate(sizeof(ToolkitDerived));
						new (base) ToolkitDerived(i);

			std.push_back(base);

						base = (ToolkitDerived *)pool->allocate(sizeof(ToolkitDerived));
						new (base) ToolkitDerived(i);

			array.emplace_back(base);

						base = (ToolkitDerived *)pool->allocate(sizeof(ToolkitDerived));
						new (base) ToolkitDerived(i);

			std.push_back(base);

						base = (ToolkitDerived *)pool->allocate(sizeof(ToolkitDerived));
						new (base) ToolkitDerived(i);

			array.emplace_back(base);

			std.back()->~ToolkitBase();
			array.back()->~ToolkitBase();

			std.pop_back();

			array.pop_back();

						base = (ToolkitBase *)pool->allocate(sizeof(ToolkitBase));
						new (base) ToolkitBase(i);
			std.push_back(base);

						base = (ToolkitBase *)pool->allocate(sizeof(ToolkitBase));
						new (base) ToolkitBase(i);

			array.emplace_back(base);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> >::iterator it = array.begin();
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch");
		}

		it = array.begin();
		iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			(*it)->~ToolkitBase();
			(*iter)->~ToolkitBase();
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

	DynArray<std::string, Nginx_PoolAllocator<std::string> >	array((Nginx_PoolAllocator<std::string>(pool)));
	
	array.push_back("uma string bue grande qye aloca memoria de certeza");

	array.clear();

	pool->destroy();

    return (testNumber);
}
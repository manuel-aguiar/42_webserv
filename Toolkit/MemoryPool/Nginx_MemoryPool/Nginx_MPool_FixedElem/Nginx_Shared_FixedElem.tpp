/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_Shared_FixedElem.tpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 10:19:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/05 22:18:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_POOLALLOCATOR_FIXEDELEM_TPP

# define NGINX_POOLALLOCATOR_FIXEDELEM_TPP

# include "Nginx_MPool_FixedElem.hpp"
#include <cstdio>

template <typename T>
class Nginx_Shared_FixedElem
{
	public:

		typedef T					value_type;
		typedef T*					pointer;
		typedef const T*			const_pointer;
		typedef T&					reference;
		typedef const T&			const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;

		template <typename U>
		struct rebind
		{
			typedef Nginx_Shared_FixedElem<U> other;
		};
		Nginx_Shared_FixedElem(Nginx_MPool_FixedElem<T>& pool) : m_memoryPool(&pool) {/*std::cout << "allcoator inituialzed: " << pool << std::endl;*/}
		Nginx_Shared_FixedElem(const Nginx_Shared_FixedElem& copy) : m_memoryPool(copy.m_memoryPool) {(void)copy;}
		Nginx_Shared_FixedElem& operator=(const Nginx_Shared_FixedElem& copy) {m_memoryPool = copy.m_memoryPool; return *this;}
		~Nginx_Shared_FixedElem() {};

		template <typename U>
		Nginx_Shared_FixedElem(const Nginx_Shared_FixedElem<U>& rebind) : 
			m_memoryPool(reinterpret_cast<Nginx_MPool_FixedElem<T>*>(rebind.m_memoryPool))
		{
			/*
				reinterpret_cast<Nginx_MPool_FixedElem<T>*>(rebind.m_memoryPool)
				i hate this C-style cast...... but it was the only way i found
			*/
		}

		pointer allocate(size_type n, const void* hint = 0)
		{
			assert (n <= max_size());

			(void)hint;
			return (static_cast<pointer>(m_memoryPool->allocate(1)));
		}

		void deallocate(pointer p, size_type n)
		{
			m_memoryPool->deallocate(p, n);
		}

		void construct(pointer p, const_reference val)
		{
			m_memoryPool->construct(p, val);
		}

		void destroy(pointer p)
		{
			m_memoryPool->destroy(p);
		}

		bool operator==(const Nginx_Shared_FixedElem& other) const { return (m_memoryPool == other.m_memoryPool); }
		bool operator!=(const Nginx_Shared_FixedElem& other) const { return (m_memoryPool != other.m_memoryPool); }

		size_type max_size() const
		{
			return m_memoryPool->max_size();
		}

	//private:
		Nginx_MPool_FixedElem<T>* m_memoryPool;
};




#endif

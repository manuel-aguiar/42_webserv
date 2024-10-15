/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 09:35:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPARRAY_TPP

# define HEAPARRAY_TPP

#include <cstring>
#include <cassert>
#include <iostream>

template <typename T>
class HeapArray
{
	public:

		HeapArray() : m_array(NULL), m_size(0)
		{
			//std::cout <<" heaparray constructed" << std::endl;
		};

		HeapArray(size_t size) : m_array(reinterpret_cast<T*>(new t_byte [sizeof(T) * size])), m_size(size)
		{
			//std::cout << "heaparray constructed parameter" << std::endl;
		};

		~HeapArray()
		{
			//std::cout << "heap array destroyed" << std::endl;
			if (m_array)
				delete [] (reinterpret_cast<t_byte*>(m_array));
		};

		HeapArray(const HeapArray &other) : m_array(reinterpret_cast<T*>(new t_byte [sizeof(T) * other.m_size])), m_size(other.m_size)
		{
			//std::cout << "heaparray copied" << std::endl;

			std::memcpy(reinterpret_cast<t_byte*>(m_array), 
						reinterpret_cast<t_byte*>(other.m_array), 
						sizeof(T) * m_size);
			
		};

		HeapArray &operator=(const HeapArray &other)
		{
			//std::cout << "heaparray assigned" << std::endl;
			if (this == &other)
				return (*this);
			if (m_size != other.m_size)
			{
				if (m_array)
					delete [] (reinterpret_cast<t_byte*>(m_array));
				m_size = other.m_size;
				m_array = reinterpret_cast<T*>(new t_byte [sizeof(T) * m_size]);
			}
			std::memcpy(reinterpret_cast<t_byte*>(m_array), 
						reinterpret_cast<t_byte*>(other.m_array), 
						sizeof(T) * m_size);
			return (*this);
		};

		T& operator[](const size_t index)
		{
			//std::cout << "index: " << index << "; size: " << m_size << std::endl;
			assert(index < m_size);
			return *(reinterpret_cast<T*>(&m_array[index]));
		}

		
		size_t size() const
		{
			return (m_size);
		}
		
	private:

		typedef unsigned char   t_byte;
		T*						m_array;
		size_t					m_size;
};




#endif
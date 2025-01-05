/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringInPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:33:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/07 11:15:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINERINPOOL_HPP

# define CONTAINERINPOOL_HPP

# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <list>
# include <set>

template<typename T>
class Nginx_PoolAllocator;

typedef std::basic_string<char, std::char_traits<char>, Nginx_PoolAllocator<char> > StringInPool;

template <typename T>
struct VectorInPool {
    typedef std::vector<T, Nginx_PoolAllocator<T> > type;
};

template <typename T, typename U, typename Comp = std::less<T> >
struct MapInPool
{
    typedef std::map<T, U, Comp, Nginx_PoolAllocator<std::pair<T, U> > > type;
};

template <typename T, typename Comp = std::less<T> >
struct SetInPool
{
    typedef std::map<T, Comp, Nginx_PoolAllocator<T> > type;
};

template <typename T>
struct ListInPool
{
    typedef std::list<T, Nginx_PoolAllocator<T> > type;
};


template <typename T, typename Allocator>
class DynArray;



template <typename T >
struct MyVectorInPool
{
    typedef DynArray<T, Nginx_PoolAllocator<T> > type;
};

#endif
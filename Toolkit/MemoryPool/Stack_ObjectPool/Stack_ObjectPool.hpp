/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack_ObjectPool.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:21:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:35:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef Stack_ObjectPool_Hpp

# define Stack_ObjectPool_Hpp

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>
# include <memory>

template <typename T, size_t Size>
class Stack_ObjectPool;

# include "Stack_ObjectPool.tpp"

#endif
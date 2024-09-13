/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultDeleters.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:24:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 12:24:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTDELETERS_HPP

# define DEFAULTDELETERS_HPP

template<typename T>
struct DefaultDeleter
{
    void operator()(T* ptr) const {
        delete ptr;
    }
};

template<typename T>
struct DefaultDeleterArray
{
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};

#endif
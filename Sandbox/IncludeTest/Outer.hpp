/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Outer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 09:55:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 10:00:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTER_HPP

# define OUTER_HPP	

#include <vector>

class Outer
{
	public:
		Outer() {}
		~Outer() {}
	private:
		class Inner;
		std::vector<Inner> m_inners;

};

#endif
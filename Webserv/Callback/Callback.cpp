/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Callback.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:42:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 13:12:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Callback.hpp"
# include <cassert>
# include <cstddef>

Callback::Callback() :
	m_data				(NULL),
	m_handler			(NULL)
{}

Callback::~Callback() {}


void Callback::execute()
{
	if (m_handler)
		(m_handler)(*this);
}

void	Callback::reset()
{
	m_data = NULL;
	m_handler = NULL;
}

//private
Callback::Callback(const Callback& copy) : m_data(copy.m_data), m_handler(copy.m_handler) {(void)copy;}
Callback& Callback::operator=(const Callback& assign) {(void)assign; return (*this);}


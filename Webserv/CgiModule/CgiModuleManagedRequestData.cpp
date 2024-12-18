/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleManagedRequestData.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 09:13:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:13:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiModule.hpp"

CgiModule::ManagedRequestData::ManagedRequestData() :
	CgiRequestData()
{
}

CgiModule::ManagedRequestData::~ManagedRequestData()
{
}

CgiModule::ManagedRequestData::ManagedRequestData(const ManagedRequestData &copy) :
	CgiRequestData(copy)
{
}

CgiModule::ManagedRequestData &CgiModule::ManagedRequestData::operator=(const ManagedRequestData &assign)
{
	if (this != &assign)
		CgiRequestData::operator=(assign);
	return (*this);
}
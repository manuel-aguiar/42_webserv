/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleGetSetAccess.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:03:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 15:01:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiModule.hpp"

const t_CgiEnvKey*						CgiModule::getBaseEnvKeys() const
{
	return (m_baseEnv);
}

const std::map<t_extension, t_path>&	CgiModule::getInterpreters() const
{
	return (m_Interpreters);
}

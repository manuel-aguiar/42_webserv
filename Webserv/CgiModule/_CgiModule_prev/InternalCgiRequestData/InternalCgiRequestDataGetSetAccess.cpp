/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiRequestDataGetSetAccess.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 10:30:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 11:13:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "InternalCgiRequestData.hpp"



CgiModule::InternalCgiRequestData::t_func_CgiHandler 
CgiModule::InternalCgiRequestData::getHandler(const e_CgiCallback type) const
{
    return (m_stateCallbacks[type]);
}

CgiModule::InternalCgiRequestData::t_ptr_CgiUser
CgiModule::InternalCgiRequestData::getUser() const
{
    return (m_user);
}
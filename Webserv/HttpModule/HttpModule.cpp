/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpModule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:10:13 by manuel            #+#    #+#             */
/*   Updated: 2024/11/28 11:10:49 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpModule.hpp"
# include "HttpConnection/HttpConnection.hpp"

void HttpModule::initConnection(Connection* accepted)
{
	HttpConnection* conn = new HttpConnection(accepted);
}

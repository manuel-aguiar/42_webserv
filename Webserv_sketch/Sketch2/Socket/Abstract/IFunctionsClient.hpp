/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFunctionsClient.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:21:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 13:39:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTFUNCTIONS_HPP

# define ICLIENTFUNCTIONS_HPP

class IClientFunctions
{
    public:
        virtual ~IClientFunctions() {}

        virtual void connect() = 0;
        virtual void disconnect() = 0;
};


#endif
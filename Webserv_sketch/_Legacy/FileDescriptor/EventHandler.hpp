/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerFunction.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:30:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 14:27:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP

# define EVENTHANDLER_HPP

# include "EPollEvent.hpp"
# include "EPollManager.hpp"

class HandlerFunction
{
    public:

        void    handleEvent(const EPollEvent& event, EPollManager& manager);
};


#endif
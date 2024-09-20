/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionalEvents.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 08:14:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/18 08:20:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONALEVENTS_HPP


# define FUNCTIONALEVENTS_HPP

typedef enum
{
    E_READ = 0,
    E_WRITE,
    E_ERROR,
    E_CLOSE,
    E_COUNT
} e_Event;




#endif
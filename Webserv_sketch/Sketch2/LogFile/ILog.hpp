/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ILog.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:22:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:52:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ILOG_HPP

# define ILOG_HPP

# include <string>

/*

    Log interface.
    Two implementations: with threads + without threads

    Dependency injection pattern: server will be built with this in mind.

*/

class ILog
{
    public:
        virtual ~ILog() {}
        virtual void record(const std::string& entry) = 0;
};


#endif
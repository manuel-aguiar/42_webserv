/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPollableFd.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:57:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:35:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLLABLEFILEDESCRIPTOR_HPP

# define IPOLLABLEFILEDESCRIPTOR_HPP

# include "FileDescriptor.hpp"
# include "IOnEvents.hpp"

class IPollableFd : public FileDescriptor, public IOnEvents
{
    public:
        virtual ~IPollableFd() {}
};


#endif
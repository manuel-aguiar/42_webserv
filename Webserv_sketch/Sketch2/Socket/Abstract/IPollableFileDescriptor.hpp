/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPollableSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:57:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:03:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLLABLEFILEDESCRIPTOR_HPP

# define IPOLLABLEFILEDESCRIPTOR_HPP

# include "../../FileDescriptor/FileDescriptor.hpp"
# include "IOnEvents.hpp"

class IPollableFileDescriptor : public FileDescriptor, public IOnEvents
{
    public:
        virtual ~IPollableFileDescriptor() {}
};


#endif
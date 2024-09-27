/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionalWebservFileDescriptor.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 08:02:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/18 08:02:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVFILEDESCRIPTOR_HPP

# define WEBSERVFILEDESCRIPTOR_HPP

class WebservFileDescriptor
{

    public:
        WebservFileDescriptor();
        WebservFileDescriptor(int WebservFileDescriptor);
        ~WebservFileDescriptor();

        int     getFd() const;

        //      to access the fd
        int     operator()() const;

        // for simple comparison
        bool    operator==(const int other) const;
        bool    operator==(const WebservFileDescriptor& other) const;

        // for STL ordering, std::lower
        bool    operator<(const int other) const;
        bool    operator<(const WebservFileDescriptor& other) const;

        bool    setCloseOnExec_NonBlocking();

        bool    redirectTo   (const int newFd);
        bool    redirectTo   (const WebservFileDescriptor& newFd);

        void        close();
        
    protected:
        int                 _fd;
        
        // hard copy and assign is only for descendents
        WebservFileDescriptor(const WebservFileDescriptor& hardCopy);
        WebservFileDescriptor& operator=(const WebservFileDescriptor& hardAssign);

        //helper
        bool                _addFlags(int flags);

};


#endif
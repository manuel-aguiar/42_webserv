/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/06 12:54:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

# include <cstddef>

# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cerrno>


class CgiRequest
{
    public:
        CgiRequest(const char** argv, const char** envp, const char* stdinData);
        ~CgiRequest();
        CgiRequest(const CgiRequest &other);
        CgiRequest &operator=(const CgiRequest &other);

        void    execute();
        void    debugPrintInputs();
        
        char**    _argv;
        char**    _envp;
        char*     _stdinData;
    
    private:
        CgiRequest();
};



#endif
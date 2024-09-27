/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:59:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "../Webserver_Definitions.h"

# include <queue>
# include <list>

# define MAX_CONCURRENT_CGI 10

class Cgi_Handler
{
    public:
        Cgi_Handler();
        ~Cgi_Handler();

    private:

        typedef struct s_cgi_data
        {
            /**** a definir **/ 
            t_byte*     _data;
            size_t      _size;
        }               t_cgi_data;

        typedef MemoryPool_Dealloc<s_cgi_data> t_cgi_pool;
        typedef std::queue<t_cgi_data, std::list<t_cgi_data, t_cgi_pool> > t_cgi_queue;

        t_cgi_queue         _queue;
        size_t              _maxConcurrent;
        size_t              _currentConcurrent;
};


#endif
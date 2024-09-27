/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:38:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "../Webserver_Definitions.h"

# include <queue>
# include <list>

# define MAX_CONCURRENT_CGI 10

class CGI_Handler
{
    public:
        CGI_Handler();
        ~CGI_Handler();

    private:

        typedef struct CGI_data
        {
            /**** a definir **/ 
            t_byte*     _data;
            size_t      _size;
        }               t_CGI_data;

        typedef MemoryPool_Dealloc<CGI_data> t_CGI_pool;
        typedef std::queue<t_CGI_data, std::list<t_CGI_data, t_CGI_pool> > CGI_Queue;

        CGI_Queue       _queue;
        size_t          _maxConcurrent;
        size_t          _currentConcurrent;
};


#endif
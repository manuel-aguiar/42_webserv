/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:43:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 16:19:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONCGI_HPP

# define PYTHONCGI_HPP

# include "../../MemoryPool/MemoryPool.h"
# include <map>

class PythonCgi
{
    public:
        PythonCgi();
        ~PythonCgi();
    

        void    init();
        void    reset();

        void    prepareCgi();

    private:

        typedef enum {
            PY_REQUEST_METHOD,       
            PY_QUERY_STRING,         
            PY_CONTENT_TYPE,         
            PY_CONTENT_LENGTH,       
            PY_SCRIPT_NAME,          
            PY_PATH_INFO,            
            PY_PATH_TRANSLATED,      
            PY_HTTP_COOKIE,          
            PY_REMOTE_ADDR,          
            PY_REMOTE_HOST,          
            PY_REMOTE_USER,          
            PY_AUTH_TYPE,            
            PY_SERVER_NAME,          
            PY_SERVER_PORT,          
            PY_SERVER_PROTOCOL,      
            PY_SERVER_SOFTWARE,      
            PY_HTTP_USER_AGENT,      
            PY_HTTP_REFERER,         
            PY_PYTHON_PATH,           
            PY_CGIENV_COUNT           
        } PyCgiEnv;

        Nginx_MemoryPool*           _localDataPool;
        Nginx_MemoryPool*           _requestDataPool;

        typedef std::map<char*, PyCgiEnv, std::less<char *>, MPool_FixedElem<std::pair<char*, PyCgiEnv> > >   mapHeaderToEnum;
        typedef mapHeaderToEnum::iterator                                                                     mapHeaderToEnum_Iter;
        typedef std::map<PyCgiEnv, char*, std::less<PyCgiEnv>, MPool_FixedElem<std::pair<PyCgiEnv, char*> > >   mapEnumToHeader;
        typedef mapHeaderToEnum::iterator                                                                     mapEnumToHeader_Iter;


        mapHeaderToEnum             _headersToEnum;
        mapEnumToHeader            _enumToHeaders;
        char*                       _RequestEnv[PY_CGIENV_COUNT];

        void    setupMapEntry(const char *entry, PyCgiEnv enumerator);

        PythonCgi(const PythonCgi &other);
        PythonCgi &operator=(const PythonCgi &other);
};



#endif
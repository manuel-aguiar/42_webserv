/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:43:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/06 11:10:51 by mmaria-d         ###   ########.fr       */
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

        void    printVariables();
        void    printEnumerators();

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
        StringAllocator<char>       _stringAllocator;

        typedef std::basic_string<char, std::char_traits<char>, StringAllocator<char> > t_poolString;

        typedef std::map<t_poolString, PyCgiEnv, std::less<t_poolString>, MPool_FixedElem<std::pair<t_poolString, PyCgiEnv> > >   mapHeaderToEnum;
        typedef mapHeaderToEnum::iterator                                                                     mapHeaderToEnum_Iter;
        typedef std::map<PyCgiEnv, t_poolString*, std::less<PyCgiEnv>, MPool_FixedElem<std::pair<PyCgiEnv, t_poolString*> > > mapEnumToHeader;
        typedef mapEnumToHeader ::iterator                                                                     mapEnumToHeader_Iter;


        mapHeaderToEnum             _headersToEnum;
        mapEnumToHeader             _enumToHeaders;
        char*                       _RequestEnv[PY_CGIENV_COUNT];

        void    setupMapEntry(const char *entry, PyCgiEnv enumerator);

        PythonCgi(const PythonCgi &other);
        PythonCgi &operator=(const PythonCgi &other);
};



#endif
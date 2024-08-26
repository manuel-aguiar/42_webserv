/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodFactory.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 07:51:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 08:36:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODFACTORY_HPP

# define HTTPMETHODFACTORY_HPP

# include <string>
# include <map>

# include "GET/HttpMethodGET.hpp"
# include "HEAD/HttpMethodHEAD.hpp"
# include "POST/HttpMethodPOST.hpp"
# include "DELETE/HttpMethodDELETE.hpp"

class HttpMethodFactory
{
    public:

        static IHttpMethod* create(const std::string& methodName);
    
    private:

        typedef IHttpMethod* (*CtorFunc)();

        typedef std::map<std::string, CtorFunc>::const_iterator mapIter;
        static const std::map<std::string, CtorFunc> ctors;


        static IHttpMethod* createGET();
        static IHttpMethod* createHEAD();
        static IHttpMethod* createPOST();
        static IHttpMethod* createDELETE();

        HttpMethodFactory();
        ~HttpMethodFactory();
        HttpMethodFactory(const HttpMethodFactory& copy);
        HttpMethodFactory& operator=(const HttpMethodFactory& assign);

};



#endif
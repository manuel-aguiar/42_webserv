/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodFactory.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:25:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 08:40:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMethodFactory.hpp"


//setting up the map of methods
const std::map<std::string, HttpMethodFactory::CtorFunc> HttpMethodFactory::ctors =
{
    std::make_pair("GET", &HttpMethodFactory::createGET),
    std::make_pair("HEAD", &HttpMethodFactory::createHEAD),
    std::make_pair("POST", &HttpMethodFactory::createPOST),
    std::make_pair("DELETE", &HttpMethodFactory::createDELETE)
};

//actually factory
IHttpMethod* HttpMethodFactory::create(const std::string& methodName)
{
    HttpMethodFactory::mapIter iter;

    iter = ctors.find(methodName);
    if (iter != ctors.end())
        return (iter->second());
    return (NULL);
}

//individual creators
IHttpMethod* HttpMethodFactory::createGET() {
    return (new HttpMethodGET());
}

IHttpMethod* HttpMethodFactory::createHEAD() {
    return (new HttpMethodHEAD());
}

IHttpMethod* HttpMethodFactory::createPOST() {
    return (new HttpMethodPOST());
}

IHttpMethod* HttpMethodFactory::createDELETE() {
    return (new HttpMethodDELETE());
}


//boilerplate
HttpMethodFactory::HttpMethodFactory() {}
HttpMethodFactory::~HttpMethodFactory() {}
HttpMethodFactory::HttpMethodFactory(const HttpMethodFactory& copy) {}
HttpMethodFactory& HttpMethodFactory::operator=(const HttpMethodFactory& assign) {return (*this);}

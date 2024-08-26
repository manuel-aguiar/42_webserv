/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodFactory.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:25:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 08:37:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMethodFactory.hpp"

const std::map<std::string, HttpMethodFactory::CtorFunc> HttpMethodFactory::ctors =
{
    std::make_pair("GET", &HttpMethodFactory::createGET),
    std::make_pair("HEAD", &HttpMethodFactory::createHEAD),
    std::make_pair("POST", &HttpMethodFactory::createPOST),
    std::make_pair("DELETE", &HttpMethodFactory::createDELETE)
};

IHttpMethod* HttpMethodFactory::create(const std::string& methodName)
{
    HttpMethodFactory::mapIter iter;

    iter = ctors.find(methodName);
    if (iter != ctors.end())
        return (iter->second());
    return (NULL);
}

IHttpMethod* HttpMethodFactory::createGET() {
    return new HttpMethodGET();
}

IHttpMethod* HttpMethodFactory::createHEAD() {
    return new HttpMethodHEAD();
}

IHttpMethod* HttpMethodFactory::createPOST() {
    return new HttpMethodPOST();
}

IHttpMethod* HttpMethodFactory::createDELETE() {
    return new HttpMethodDELETE();
}

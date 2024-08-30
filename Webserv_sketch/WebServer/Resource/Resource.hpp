/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:56:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/30 14:05:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_HPP

# define RESOURCE_HPP

/*
    Resource is what is considered to be a website (directory tree).
    A server that "serves" content from multiple websites (directory trees), sees individual
    websites as "Resources"
*/


class Resource
{
    public:
        Resource();
        ~Resource();
        Resource(const Resource& copy);
        Resource& operator=(const Resource& assign);
    

    private:
        
};



#endif
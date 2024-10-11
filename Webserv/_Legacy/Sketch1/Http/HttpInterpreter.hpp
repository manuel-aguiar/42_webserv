/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpInterpreter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 08:14:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 11:21:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPINTERPRETER_HPP

# define HTTPINTERPRETER_HPP

class HttpInterpreter
{
    public:
        HttpInterpreter();
        ~HttpInterpreter();

        HttpInterpreter(const HttpInterpreter& copy);
        HttpInterpreter& operator=(const HttpInterpreter& assign);

        void    interpret();
};



#endif
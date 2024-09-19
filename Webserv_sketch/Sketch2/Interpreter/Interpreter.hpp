/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interpreter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/18 10:32:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_HPP

# define INTERPRETER_HPP

# include "IInterpreter.hpp"

class HttpInterpreter : public IInterpreter
{
    public:
        HttpInterpreter();
        ~HttpInterpreter();

        HttpInterpreter(const HttpInterpreter& copy);
        HttpInterpreter& operator=(const HttpInterpreter& assign);

        void    interpret();
};



#endif
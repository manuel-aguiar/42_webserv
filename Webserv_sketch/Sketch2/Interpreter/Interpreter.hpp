/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interpreter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 12:12:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_HPP

# define INTERPRETER_HPP

# include "IInterpreter.hpp"

class Interpreter : public IInterpreter
{
    public:
        Interpreter();
        ~Interpreter();

        Interpreter(const Interpreter& copy);
        Interpreter& operator=(const Interpreter& assign);

        void    interpret();
};



#endif
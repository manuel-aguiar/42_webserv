/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interpreter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 10:45:09 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_HPP

# define INTERPRETER_HPP

# include "IInterpreter.hpp"

class HttpManager : public IInterpreter
{
    public:
        HttpManager();
        ~HttpManager();

        HttpManager(const HttpManager& copy);
        HttpManager& operator=(const HttpManager& assign);

        void    interpret();
};



#endif

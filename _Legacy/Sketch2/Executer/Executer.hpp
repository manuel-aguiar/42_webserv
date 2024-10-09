/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 12:11:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Executer_HPP

# define Executer_HPP

# include "IExecuter.hpp"

class Executer : public IExecuter
{
    public:
        Executer();
        ~Executer();

        Executer(const Executer& copy);
        Executer& operator=(const Executer& assign);

        void execute();
};



#endif
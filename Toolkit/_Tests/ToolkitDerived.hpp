/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitDerived.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:07:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:10:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLKITDERIVED_HPP

# define TOOLKITDERIVED_HPP

#include "ToolkitBase.hpp"

class ToolkitDerived : public ToolkitBase
{
    public:
        ToolkitDerived();
        ToolkitDerived(int value);
        ToolkitDerived(const ToolkitDerived& other);
        ToolkitDerived& operator=(const ToolkitDerived& other);
        ~ToolkitDerived();

        bool operator==(const ToolkitDerived& other);
        bool operator!=(const ToolkitDerived& other);

        void print();
};


#endif
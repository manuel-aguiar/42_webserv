/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitBase.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:07:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 09:15:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLKITBASE_HPP

# define TOOLKITBASE_HPP

#include <string>
#include <cstring>
#include <iostream>

class ToolkitBase
{
    public:
        ToolkitBase();
        ToolkitBase(int value);
        ToolkitBase(const ToolkitBase& other);
        ToolkitBase& operator=(const ToolkitBase& other);
        virtual ~ToolkitBase();

        bool operator==(const ToolkitBase& other) const;
        bool operator!=(const ToolkitBase& other) const;

        virtual void print();

    protected:
        int* m_data;
        int value;
        std::string _name;
};

#endif
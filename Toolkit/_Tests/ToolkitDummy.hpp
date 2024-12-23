/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitDummy.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:00:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:26:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef TOOLKITDUMMY_HPP

# define TOOLKITDUMMY_HPP

//C++ headers
# include <string>
# include <cstring>

class ToolkitDummy
{
    public:
        ToolkitDummy();
        ToolkitDummy(int value);
        ~ToolkitDummy();
        ToolkitDummy(const ToolkitDummy& other);
        ToolkitDummy& operator=(const ToolkitDummy& other);

        bool operator==(const ToolkitDummy& other);
        bool operator!=(const ToolkitDummy& other);

        const char* print() const;

    private:
        int             value;
        int*            m_data;
        std::string     _name;
};

std::ostream& operator<<(std::ostream& os, const ToolkitDummy& dummy);

#endif
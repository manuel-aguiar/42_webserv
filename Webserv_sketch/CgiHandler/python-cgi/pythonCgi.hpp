/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:43:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 12:45:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONCGI_HPP

# define PYTHONCGI_HPP


class PythonCgi
{
    public:
        PythonCgi();
        ~PythonCgi();
    

    private:
        PythonCgi(const PythonCgi &other);
        PythonCgi &operator=(const PythonCgi &other);
};



#endif
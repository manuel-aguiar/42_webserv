/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phpCgi.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 12:45:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHPCGI_HPP

# define PHPCGI_HPP


class PhpCgi
{
    public:
        PhpCgi();
        ~PhpCgi();
    



    private:
        PhpCgi(const PhpCgi &other);
        PhpCgi &operator=(const PhpCgi &other);
};

#endif
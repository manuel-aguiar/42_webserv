/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phpFastCgi_Fpm.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 12:45:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHPFASTCGI_FPM_HPP

# define PHPFASTCGI_FPM_HPP

class PhpFastCgi_Fpm
{
    public:
        PhpFastCgi_Fpm();
        ~PhpFastCgi_Fpm();
    

    private:
        PhpFastCgi_Fpm(const PhpFastCgi_Fpm &other);
        PhpFastCgi_Fpm &operator=(const PhpFastCgi_Fpm &other);
};





#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:19:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:55:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITHREADTASK_HPP

# define ITHREADTASK_HPP

class IThreadTask
{
    public:
        virtual ~IThreadTask() {};
        virtual void            execute() const  = 0;
        virtual IThreadTask*    clone() const    = 0;
};


#endif
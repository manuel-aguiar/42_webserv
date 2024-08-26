/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:19:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 15:05:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITHREADTASK_HPP

# define ITHREADTASK_HPP

class IThreadTask
{
    virtual ~IThreadTask() {};
    virtual void            execute() = 0;
    virtual IThreadTask*    clone() = 0;
};


#endif
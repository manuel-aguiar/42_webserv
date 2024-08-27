/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndependentTask.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 14:43:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDEPENDENTTASK_HPP

# define INDEPENDENTTASK_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>
# include <cassert>

# include "../Abstract/IThreadTask.hpp"
# include <vector>

class IndependentTask : public IThreadTask
{
    public:
        IndependentTask(std::vector<int>& vec, int index);
        ~IndependentTask();
        IndependentTask(const IndependentTask& copy);
        IndependentTask& operator=(const IndependentTask& assign);

        void            execute() const;
        IThreadTask*    clone() const;

    private:
        std::vector<int>& _vec;
        int               _index;

        
        

};

#endif
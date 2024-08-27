/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndependentTask.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 09:27:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADTASK_HPP

# define THREADTASK_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>
# include <cassert>

# include "IThreadTask.hpp"
# include <vector>

class IndependentTask : public IThreadTask
{
    public:
        IndependentTask(std::vector<int>& vec, int index);
        ~IndependentTask();
        IndependentTask(const IndependentTask& copy);

        void            execute() const;
        IThreadTask*    clone() const;

    private:
        std::vector<int>& _vec;
        int               _index;

        IndependentTask& operator=(const IndependentTask& assign);
        

};

#endif
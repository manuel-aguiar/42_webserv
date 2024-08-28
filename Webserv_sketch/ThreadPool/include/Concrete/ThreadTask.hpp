/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/28 08:21:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ThreadTask_HPP

# define ThreadTask_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>
# include <cassert>

# include "../Abstract/IThreadTask.hpp"
# include <vector>

class ThreadTask : public IThreadTask
{
    public:
        ThreadTask(std::vector<int>& vec, int index);
        ~ThreadTask();
        ThreadTask(const ThreadTask& copy);
        ThreadTask& operator=(const ThreadTask& assign);

        void            execute() const;
        IThreadTask*    clone() const;

    private:
        std::vector<int>& _vec;
        int               _index;
};

#endif
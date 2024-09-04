/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   IndependentTask.cpp						  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 09:09:48 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/27 08:56:38 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "IndependentTask.hpp"

IndependentTask::IndependentTask(std::vector<int>& vec, int index) : 
	_vec(vec),
	_index(index)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "IndependentTask Constructor Called" << std::endl;
	#endif
}

IndependentTask::~IndependentTask()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "IndependentTask dSTRUCTOR Called" << std::endl;
	#endif
}

IndependentTask::IndependentTask(const IndependentTask& copy) :
	_vec(copy._vec),
	_index(copy._index)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "IndependentTask Constructor Called" << std::endl;
	#endif
}

IndependentTask&  IndependentTask::operator=(const IndependentTask& copy)
{
	if (this == &copy)
		return (*this);
	*this = copy;
	return (*this);
}

void	IndependentTask::execute() const
{
	_vec[_index] = _index;
}

IThreadTask*	IndependentTask::clone() const
{
	return (new IndependentTask(*this));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:11:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 15:21:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H

# define TEST_H

# include <iostream>
# include <iomanip>

#define TEST_FAIL_INFO() \
	std::cerr   << std::setw(10) << "	File: " << __FILE__ << "\n" \
				<< std::setw(10) << "	Line: " << __LINE__ << "\n" \
				<< std::setw(10) << "	Function: " << __FUNCTION__ << std::endl;

#endif
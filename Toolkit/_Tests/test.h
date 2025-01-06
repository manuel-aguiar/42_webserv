/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:11:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 10:51:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H

# define TEST_H

# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string FileLineFunction(const char* file, const int line, const char* function);

#define TEST_FAIL_INFO() \
	std::cerr   << std::setw(10) << "	File: " << __FILE__ << "\n" \
				<< std::setw(10) << "	Line: " << __LINE__ << "\n" \
				<< std::setw(10) << "	Function: " << __FUNCTION__ << std::endl;



#endif
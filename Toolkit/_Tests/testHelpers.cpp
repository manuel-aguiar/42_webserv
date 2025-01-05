/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testHelpers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 10:44:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 10:51:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "test.h"

std::string FileLineFunction(const char* file, const int line, const char* function)
{
    return std::string("\tFile: ") + file + "\n\tLine: " + to_string(line) + "\n\tFunction: " + function + '\n';
}
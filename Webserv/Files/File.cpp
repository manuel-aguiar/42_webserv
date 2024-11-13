/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:25:40 by codespace         #+#    #+#             */
/*   Updated: 2024/11/11 14:34:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

File::File(const char *path)
{
	(void)path;
}

File::~File()
{

}

File::File(const File &copy)
{
	(void)copy;
}

File &File::operator=(const File &copy)
{
	(void)copy;
	return (*this);
}



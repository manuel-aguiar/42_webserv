/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:28:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:39:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../LogFile/LogFile.hpp"
#include "../Globals.hpp"

LogFile::LogFile(const char* filename, Globals* globals)
{
	(void)filename;
    (void)globals;
}

LogFile::~LogFile()
{
}

void	LogFile::setGlobals(Globals& globals)
{
	(void)globals;
}

void	LogFile::record(const std::string& entry)
{
    (void)entry;
}

void	LogFile::record(const char* entry)
{
	(void)entry;
}

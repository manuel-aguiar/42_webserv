/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilesUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:26:14 by codespace         #+#    #+#             */
/*   Updated: 2024/11/11 14:29:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILESUTILS_HPP

# define FILESUTILS_HPP

# include <sys/stat.h>

class FilesUtils
{
	public:
		static bool fileExists(const char* path);
		static bool isDirectory(const char* path);
		static bool isFile(const char* path);
};

#endif

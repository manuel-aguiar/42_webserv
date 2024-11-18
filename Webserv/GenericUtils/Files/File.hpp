/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:26:27 by codespace         #+#    #+#             */
/*   Updated: 2024/11/11 14:33:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP

# define FILE_HPP

#include "../Webserver_Definitions.h"
#include <unistd.h>
#include <fcntl.h>

class File
{
	public:
		File(const char* path);
		~File();

		bool		read(void* buffer, size_t size);
		bool		write(const void* buffer, size_t size);
		size_t		size() const;
		const char* path() const;

	private:
		t_fd		m_fd;
		size_t 		m_size;
		size_t		offset;

		File(const File& copy);
		File& operator=(const File& copy);

};

#endif

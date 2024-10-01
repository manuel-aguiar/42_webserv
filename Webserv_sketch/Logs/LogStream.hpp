/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:55:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 07:46:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGSTREAM_HPP

# define LOGSTREAM_HPP

# include "ILog.hpp"

# include <iostream>
# include <cerrno>

class LogStream : public ILog
{
    public:
        LogStream(std::ostream& stream);
        ~LogStream();

        void    record (const std::string& entry);
        void    record(const char* entry);
    private:
        std::ostream&   _stream;
};



#endif
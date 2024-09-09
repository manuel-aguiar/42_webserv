/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:55:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 11:04:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGSTREAM_HPP

# define LOGSTREAM_HPP

# include "ILog.hpp"

class LogStream : public ILog
{
    public:
        LogStream(std::ostream& stream);
        ~LogStream();

        void record(const std::string& entry);
    
    private:
        std::ostream&   _stream;
};



#endif
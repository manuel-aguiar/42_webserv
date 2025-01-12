/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiStressTest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:20:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 00:35:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGISTRESSTEST_HPP

# define CGISTRESSTEST_HPP

# include <string>

class A_ProtoRequest;

class CgiStressTest
{
    public:
        static void    ValidPython(A_ProtoRequest& proto);
        static void    ValidShell(A_ProtoRequest& proto);
        static void    ValidPHP(A_ProtoRequest& proto);
        static void    InvalidScript(A_ProtoRequest& proto);
        static void    InvalidInterpreter(A_ProtoRequest& proto);


        // Assignment Criteria, between scripts, bad interpreters, a bit of everything
        static void    MixedCriteria        (A_ProtoRequest& proto, int index);
        static void    AllValidCriteria   (A_ProtoRequest& proto, int index);
        static void    AllInvalidCriteria   (A_ProtoRequest& proto, int index);
        
        static std::string 
                        ValgrindReadandClear(const char* filename);
        static int     StressTest(int testNumber, 
                                const int workers, 
                                const int backlog, 
                                const int connectionCount, 
                                const unsigned int timeoutMs,
                                void (*AssignmentCriteria)(A_ProtoRequest& proto, int index),
                                const char* assignmentDescription);
        
        static void prepareExpectedOutput(bool isExpectedValid, A_ProtoRequest& proto);
};



#endif
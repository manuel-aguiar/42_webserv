/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiStressTest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:20:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 18:30:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGISTRESSTEST_HPP

# define CGISTRESSTEST_HPP

class A_ProtoRequest;

class CgiStressTest
{
    public:
        static void    ValidPython(A_ProtoRequest& proto);
        static void    ValidShell(A_ProtoRequest& proto);
        static void    ValidPHP(A_ProtoRequest& proto);
        static void    InvalidScript(A_ProtoRequest& proto);
        static void    InvalidInterpreter(A_ProtoRequest& proto);

        static void    MixedCriteria(A_ProtoRequest& proto, int index);
        static void    AllSuccessCriteria(A_ProtoRequest& proto, int index);
        static void    AllInvalidCriteria(A_ProtoRequest& proto, int index);
        
        static int     Impl_StressTest(int testNumber, 
                                const int workers, 
                                const int backlog, 
                                const int connectionCount, 
                                const unsigned int timeoutMs,
                                void (*AssignmentCriteria)(A_ProtoRequest& proto, int index));
        
        static void prepareExpectedOutput(bool isExpectedValid, A_ProtoRequest& proto);
};



#endif
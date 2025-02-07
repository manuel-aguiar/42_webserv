/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 07:43:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/02/07 16:47:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>


class Socket;
class HttpManager;

typedef class Socket t_socket;
typedef void*   t_interpreter;
typedef void (*t_interpreterRead)   (const t_socket& me);
typedef void (*t_socketRead)        (const t_socket& me, t_interpreterRead);

class Socket
{
    public:
        Socket( t_socketRead    sockRead, t_interpreterRead interpRead, t_interpreter interpreter) :
                _sockVar(42),
                _interpreter(interpreter),
                _sockRead(sockRead),
                _interpRead(interpRead) {}

        void    onRead()
        {
            _sockRead(*this, _interpRead);
        }

        int                 _sockVar;

        t_interpreter       _interpreter;
        t_socketRead        _sockRead;
        t_interpreterRead   _interpRead;

};


template <typename ExpectedType>
struct TypeCheck {
    static void check(const t_socket& me) {
        ExpectedType* interp = static_cast<ExpectedType*>(me._interpreter);
        (void)interp;
    }
};


class HttpManager
{
    public:

        HttpManager() : m_state(404) {}
        void setState(int state) { m_state = state; }
        int getState() { return m_state; }

        void    onRead(const Socket& me) {std::cout << "Http onRead: "  << m_state << " socket sockVar: " << me._sockVar << std::endl;}

    private:
        int m_state;
};

class HttpInterpreteronRead
{
    public:
        static void onRead(const Socket& me)
        {

            HttpManager* local = static_cast<HttpManager*>(me._interpreter);
            local->onRead(me);
        }
};

class FtpInterpreter
{
    public:
        FtpInterpreter() : m_state(212) {}

        void    setState(int state) { m_state = state; }
        int     getState() { return m_state; }

        void    onRead(const Socket& me) {std::cout << "Ftp onRead: "  << m_state  << " socket sockVar: " << me._sockVar << std::endl;}

    private:
        int m_state;
};

class FtpInterpreterGetter
{
    public:
        static void onRead(const Socket& me)
        {
            FtpInterpreter* local = static_cast<FtpInterpreter*>(me._interpreter);
            local->onRead(me);
        }
};

class ClientFunc
{
    public:
        static void onRead(const Socket& me, t_interpreterRead interpRead)
        {
            interpRead(me);
        }
};

class ServerFunc
{
    public:
        static void onRead(const t_socket& me, t_interpreterRead interpRead)
        {
            interpRead(me);
        }
};


typedef Socket t_serversocket;
typedef Socket t_clientsocket;

int main(void)
{
    std::vector<Socket> sockets;

    HttpManager http;
    FtpInterpreter ftp;

    t_serversocket b1(&ServerFunc::onRead, &HttpInterpreteronRead::onRead, &http);
    t_clientsocket b2(&ClientFunc::onRead, &FtpInterpreterGetter::onRead, &http);

    sockets.push_back(b1);
    sockets.push_back(b2);

    for (unsigned int i = 0; i < sockets.size(); i++)
        sockets[i].onRead();

    return 0;
}




#include <iostream>

class Socket
{
public:
    virtual ~Socket() {}
    virtual void f() = 0;
};

class Derived1 : public Socket
{
public:
    void f() {}
};

class Derived2 : public Socket
{
public:
    void f() {}
};

int main(void)
{
    Socket *b1 = new Derived1();
    Socket *b2 = new Derived2();

    for (int i = 0; i < 1000000; i++)
    {
        b1->f();
        b2->f();
    }

    delete b1;
    delete b2;

    return 0;
}
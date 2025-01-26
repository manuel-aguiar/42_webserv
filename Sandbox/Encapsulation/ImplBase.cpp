
# include "ImplBase.hpp"
# include "Worker.hpp"
# include <iostream>


ImplBase::ImplBase()
{
    m_workers.push_back(Worker(*this));
}

ImplBase::~ImplBase()
{
}

void ImplBase::publicMethod()
{
    m_workers[0].WorkerMethod();
    std::cout << "public method called" << std::endl;
}

void ImplBase::workerCalls(Worker& worker)
{
    (void)worker;
    std::cout << "worker called" << std::endl;
}
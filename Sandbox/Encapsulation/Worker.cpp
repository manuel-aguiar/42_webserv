
# include "Worker.hpp"
# include "ImplBase.hpp"
# include <iostream>

Worker::Worker(ImplBase& impl) : m_impl(impl)
{
}

Worker::~Worker()
{
}

void Worker::WorkerMethod()
{
    m_impl.workerCalls(*this);
    std::cout << "WorkerMethod called" << std::endl;
}


# include "ImplDerived.hpp"
# include "Worker.hpp"
# include <iostream>

ImplDerived::ImplDerived() : ImplBase()
{
}

ImplDerived::~ImplDerived()
{

}

void ImplDerived::publicMethod()
{
    ImplBase::publicMethod();
}

void ImplDerived::workerCalls(Worker& worker)
{
    ImplBase::workerCalls(worker);
}
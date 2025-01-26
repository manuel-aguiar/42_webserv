

#ifndef IMPLDERIVED_HPP

# define IMPLDERIVED_HPP

# include "ImplBase.hpp"

class Worker;

class ImplDerived : public ImplBase
{
    protected:
        ImplDerived();
        ~ImplDerived();

        void publicMethod();
        void HelperMethod();
        void workerCalls(Worker& worker);
    
};



#endif
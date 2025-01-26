
#ifndef PUBLIC_HPP

# define PUBLIC_HPP

# include "ImplDerived.hpp"

class Public : private ImplDerived
{
    public:
        Public();
        ~Public();
        
        void publicMethod();
    
};




#endif
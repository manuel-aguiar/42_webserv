

#ifndef _TEST_DEPENDENCIES_HPP

# define _TEST_DEPENDENCIES_HPP

# include "../../Ws_Namespace.h"
# include <vector>

Ws::Sock::addr_in 	createSockAddr_in(const std::string& ip, const std::string& port);
void    			prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners);


#endif
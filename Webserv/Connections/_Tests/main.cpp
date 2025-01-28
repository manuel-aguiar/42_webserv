

# include "../Manager/Manager.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../ServerContext/ServerContext.hpp"

#include <iostream>

int main(void)
{
	std::vector<Ws::BindInfo> 	bindAddresses;
	Globals 					globals(NULL, NULL, NULL, NULL);
	Events::Manager 			eventManager(200, globals);
	ServerContext 				context;

	bindAddresses.push_back((Ws::BindInfo){});
	bindAddresses.push_back((Ws::BindInfo){});
	bindAddresses.push_back((Ws::BindInfo){});
	bindAddresses.push_back((Ws::BindInfo){});

	Conn::Manager connManager(100, bindAddresses, eventManager, globals, context);

	connManager.init();
	connManager.shutdown();

	return (0);
}
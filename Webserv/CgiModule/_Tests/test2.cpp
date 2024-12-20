

# include "../CgiModule/CgiModule.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"

# include "TestDependencies.hpp"
# include "TestProtoConnections/A_ProtoConn.hpp"
# include <iostream>

/************************************ */
/*	Test using the actual EventManager*/	
/************************************ */

#include <unistd.h>

int main(void)
{
	std::cout << "Test1: ";
	try
	{
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager manager(globals);
		A_ProtoConn request(manager, globals);

		CgiModule cgi(10, 100, globals);

		CgiRequestData* requestData1 = cgi.acquireRequestData();

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	



	return (0);
}



# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

# include "../ImplManager/ImplManager.hpp"


namespace Conn
{
	class Manager : public ImplManager
	{
		public:
			Manager(size_t maxConnections, Events::Manager& em, Globals& globals);
			~Manager();
	};
}


#endif
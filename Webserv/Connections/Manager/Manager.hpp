
# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

# include "../ImplManager/ImplManager.hpp"


namespace Conn
{
	class Manager : public ImplManager
	{
		public:
			Manager(const size_t maxConnections,
					const std::vector<Ws::BindInfo>& bindAddresses,
					Events::Manager& eventManager, 
					ServerContext& context);
			~Manager();
	};
}


#endif
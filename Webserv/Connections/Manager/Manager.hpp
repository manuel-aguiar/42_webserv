
# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

# include "../InternalManager/InternalManager.hpp"


namespace Conn
{
	class Manager : private InternalManager
	{
		public:
			Manager(const size_t maxConnections,
					const std::vector<Ws::BindInfo>& bindAddresses,
					Events::Manager& eventManager,
					Globals& globals,
					ServerContext& context);
			~Manager();

			bool	init();
			void	shutdown();
		
		private:
			Manager(const Manager& copy);
			Manager& operator=(const Manager& assign);
	};
}


#endif

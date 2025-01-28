

#include "Manager.hpp"

namespace Conn
{
	Manager::Manager(const size_t maxConnections,
					const std::vector<Ws::BindInfo>& bindAddresses,
					Events::Manager& eventManager,
					Globals& globals,
					ServerContext& context) :
		ImplManager(maxConnections, bindAddresses, eventManager, globals, context)
	{
	}

	Manager::~Manager()
	{
	}

	bool	Manager::init()
	{
		return (ImplManager::init());
	}

	void	Manager::shutdown()
	{
		ImplManager::shutdown();
	}
}

# include "ServerContext.hpp"

ServerContext::ServerContext() :
	m_config(NULL),
	m_globals(NULL),
	m_appLayerState(NULL),
	m_appLayerInit(NULL),
	m_addonLayer(NULL) {}

ServerContext::~ServerContext() {}

ServerContext::ServerContext(const ServerContext& copy) :
	m_config(copy.m_config),
	m_globals(copy.m_globals),
	m_appLayerState(copy.m_appLayerState),
	m_appLayerInit(copy.m_appLayerInit),
	m_addonLayer(copy.m_addonLayer) {}

ServerContext& ServerContext::operator=(const ServerContext& assign)
{
	if (this == &assign)
		return (*this);

	m_config = assign.m_config;
	m_globals = assign.m_globals;
	m_appLayerState = assign.m_appLayerState;
	m_appLayerInit = assign.m_appLayerInit;
	m_addonLayer = assign.m_addonLayer;

	return (*this);
}

void	ServerContext::setServerConfig(ServerConfig& config)
{
	m_config = &config;
}

void	ServerContext::setGlobals(Globals& globals)
{
	m_globals = &globals;
}

void	ServerContext::setBlockFinder(BlockFinder& blockFinder)
{
	m_blockFinder = &blockFinder;
}

void	ServerContext::setAppLayer(Ws::AppLayer::Type type, Ws::AppLayer::Module state, Ws::AppLayer::Init init)
{
	m_appLayerState[type] = state;
	m_appLayerInit[type] = init;
}

void	ServerContext::setAddonLayer(Ws::AddonLayer::Type type, Ws::AddonLayer::State state)
{
	m_addonLayer[type] = state;
}

ServerConfig*		ServerContext::getServerConfig() const
{
	return (m_config);
}

Globals*			ServerContext::getGlobals() const
{
	return (m_globals);
}

BlockFinder*		ServerContext::getBlockFinder() const
{
	return (m_blockFinder);
}

Ws::AppLayer::Module		ServerContext::getAppLayerModule(Ws::AppLayer::Type type) const
{
	return (m_appLayerState[type]);
}

Ws::AppLayer::Init		ServerContext::getAppLayerInit(Ws::AppLayer::Type type) const
{
	return (m_appLayerInit[type]);
}

Ws::AddonLayer::State	ServerContext::getAddonLayer(Ws::AddonLayer::Type type) const
{
	return (m_addonLayer[type]);
}

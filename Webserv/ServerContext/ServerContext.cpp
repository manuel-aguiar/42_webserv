
# include "ServerContext.hpp"

namespace Server
{
	Context::Context() :
		m_config(NULL),
		m_globals(NULL),
		m_appLayerState(NULL),
		m_appLayerInit(NULL),
		m_addonLayer(NULL) {}

	Context::~Context() {}

	Context::Context(const Context& copy) :
		m_config(copy.m_config),
		m_globals(copy.m_globals),
		m_appLayerState(copy.m_appLayerState),
		m_appLayerInit(copy.m_appLayerInit),
		m_addonLayer(copy.m_addonLayer) {}
	
	Context& Context::operator=(const Context& assign)
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

	void	Context::setServerConfig(ServerConfig* config)
	{
		m_config = config;
	}

	void	Context::setGlobals(Globals* globals)
	{
		m_globals = globals;
	}

	void	Context::setAppLayer(AppLayer::Type type, AppLayer::State state, AppLayer::Init init)
	{
		m_appLayerState[type] = state;
		m_appLayerInit[type] = init;
	}

	void	Context::setAddonLayer(AddonLayer::Type type, AddonLayer::State state)
	{
		m_addonLayer[type] = state;
	}

	ServerConfig*		Context::getServerConfig() const
	{
		return (m_config);
	}

	Globals*			Context::getGlobals() const
	{
		return (m_globals);
	}

	AppLayer::State		Context::getAppLayerState(AppLayer::Type type) const
	{
		return (m_appLayerState[type]);
	}

	AppLayer::Init		Context::getAppLayerInit(AppLayer::Type type) const
	{
		return (m_appLayerInit[type]);
	}

	AddonLayer::State	Context::getAddonLayer(AddonLayer::Type type) const
	{
		return (m_addonLayer[type]);
	}
}
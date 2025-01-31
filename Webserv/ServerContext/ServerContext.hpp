

#ifndef SERVERCONTEXT_HPP

# define SERVERCONTEXT_HPP

# include "../../Toolkit/Arrays/StackArray/StackArray.hpp"

# include "../Ws_Namespace.h"

//forward declarations
class ServerConfig;
class Globals;

class ServerContext
{
	public:
		ServerContext();
		~ServerContext();
		ServerContext(const ServerContext& copy);
		ServerContext& operator=(const ServerContext& assign);

		void					setServerConfig(ServerConfig* config);
		void					setGlobals(Globals* globals);
		void					setAppLayer(Ws::AppLayer::Type type, Ws::AppLayer::Module state, Ws::AppLayer::Init init);
		void					setAddonLayer(Ws::AddonLayer::Type type, Ws::AddonLayer::State state);

		ServerConfig*			getServerConfig() const;
		Globals*				getGlobals() const;
		Ws::AppLayer::Module	getAppLayerModule(Ws::AppLayer::Type type) const;
		Ws::AppLayer::Init		getAppLayerInit(Ws::AppLayer::Type type) const;
		Ws::AddonLayer::State	getAddonLayer(Ws::AddonLayer::Type type) const;

	private:
		ServerConfig*												m_config;
		Globals*													m_globals;
		StackArray<Ws::AppLayer::Module,	Ws::AppLayer::COUNT>	m_appLayerState;
		StackArray<Ws::AppLayer::Init, 		Ws::AppLayer::COUNT>	m_appLayerInit;
		StackArray<Ws::AddonLayer::State, 	Ws::AddonLayer::COUNT>	m_addonLayer;
};


#endif
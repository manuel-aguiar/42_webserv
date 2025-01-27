

#ifndef SERVERCONTEXT_HPP

# define SERVERCONTEXT_HPP

# include "../../Toolkit/Arrays/StackArray/StackArray.hpp"

//forward declarations
class ServerConfig;
class Globals;
class Connection;

namespace Server
{
	namespace AppLayer
	{
		typedef enum
		{
			HTTP = 0,
			COUNT
		}	Type;
		typedef void* State;
		typedef void (*Init)(State, Connection*);
	}

	namespace AddonLayer
	{
		typedef enum
		{
			CGI = 0,
			COUNT
		}	Type;
		typedef void* State;
	}

	class Context
	{
		public:
			Context();
			~Context();
			Context(const Context& copy);
			Context& operator=(const Context& assign);

			void				setServerConfig(ServerConfig* config);
			void				setGlobals(Globals* globals);
			void				setAppLayer(AppLayer::Type type, AppLayer::State state, AppLayer::Init init);
			void				setAddonLayer(AddonLayer::Type type, AddonLayer::State state);

			ServerConfig*		getServerConfig() const;
			Globals*			getGlobals() const;
			AppLayer::State		getAppLayerState(AppLayer::Type type) const;
			AppLayer::Init		getAppLayerInit(AppLayer::Type type) const;
			AddonLayer::State	getAddonLayer(AddonLayer::Type type) const;

		private:
			ServerConfig*										m_config;
			Globals*											m_globals;
			StackArray<AppLayer::State, AppLayer::COUNT>		m_appLayerState;
			StackArray<AppLayer::Init, AppLayer::COUNT>			m_appLayerInit;
			StackArray<AddonLayer::State, AddonLayer::COUNT>	m_addonLayer;
	};
}


#endif
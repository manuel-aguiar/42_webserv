

#ifndef CONNECTIONLISTENER_HPP

# define CONNECTIONLISTENER_HPP

# include "../../Ws_Namespace.h"

# include "../ConnInfo/ConnInfo.hpp"

namespace Ws { namespace Listen { typedef int backlog; } }

class Listener
{
	public:
		Listener(const Ws::BindInfo& socketInfo);
		~Listener();
		Listener(const Listener& copy);
		Listener& operator=(const Listener& assign);

		int						open();
		void					close();
		int						accept(ConnInfo& accept);

		const ConnInfo&			getConnInfo() const;

	protected:
		ConnInfo				m_info;

};

#endif



# include "../../HttpResponse.hpp"

# include "../../../../ServerContext/ServerContext.hpp"
# include "../../../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"


namespace Http
{
	bool
	Response::mf_validateHeaders()
	{
		m_processFunction = &Response::mf_processBodyUpload;
		return (true);
	}
}

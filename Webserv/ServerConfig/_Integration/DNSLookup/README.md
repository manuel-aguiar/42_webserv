
Changes applied to Webserver_Definitions.h

1) added struct BindAddress

    typedef struct s_BindAddress
    {
        u_sockaddr			sockaddr;
        t_socklen			addrlen;
    }	BindAddress; 

    It encompasses information required by the ServerManager/ListeningSocket to successfully bind and listen

    The future objective will be to turn the full file into a namespace such as to organize the scope of typedefs

###########################################################################

Changes applied to ServerConfig:


1) ServerConfig now stores a straight std::vector<ServerBlock> so
this prototype was changed to:

const std::vector<ServerBlock>&				getServerBlocks() const;

2) added helper function mf_listenDNSLookup and file ServerConfigDNSLookup.cpp
    does the dns lookup for the addresses passed via the conf file

3) added this public method
    const std::vector<BindAddress>&				getAllBindAddresses() const;

    to return all bindable addresses, will be used by the servermanager to setup listening sockets

##########################################################################

Changes applied to ServerBlock:

1) added a vector of pointers to their own listeners sockaddr for match for the blockfinder:
    and the following methods:

		void							                    addListenSockAddr(const struct sockaddr* addr);         //helper
		const std::vector<const struct sockaddr*>&		    getListenSockAddr() const;                              //getter

    The getter will be used by the BlockFinder to retrieve all the sockaddresses representing were
    the server will serve connections, to setup its internal data structures for matching client requests

    Internally, the pointers stored in the vector point to the relevant BindAddress::u_sockaddr, stored inside
    the ServerConfig

###############################################

Design considerations of ServerConfigDNSLookup.cpp

	All in one go, to be fancy and unmaintainable.
	We loop through all the listen directives of each server block, one at a time.
	If it is new, update our uniqueListeners (avoid repitition), do the dns lookup,
	update our uniqueAddrInfo (avoid repitition), create the BindAddress struct,
	which takes sockaddr and socklen_t (needed by ServerManager::ListeningSocket::bind())

	If, on the other hand, the listener is not unique and we found it before, look
	at BindAddress* associated with this listener and add them back to the ServerBlocks.m_myListenSockAddr

	At the end, we will have a list of unique BindAddress structs, so we swap the one on the helper
	with the one on the ServerConfig (why copying..)

	We ARE NOT doing conversion of network to host byte order: sockets accepted by accept() are in network byte order
	as well, the match will be done in network order format. The lsiteningsockets are also setup
	based on network byte order, so avoids the "deconverting" step.
	Should the user want to print port and IP, they have to do THE CONVERTION TO HOST THEMSELVES VIA:

		/////IPV4 SPECIFIC PART, SHOUD BE SWITCHED IF WE ADDED IPV6////////
		struct sockaddr_in *addr = (struct sockaddr_in*)&address.sockaddr;
		addr->sin_port			= ::ntohs(addr->sin_port);
		addr->sin_addr.s_addr 	= ::ntohl(addr->sin_addr.s_addr);
		///////////////////////////////////////////////////////////////////	


	We must free the addrinfo structs we copied the data from (getaddrinfo allocates these for us).
	We placed the ::freeaddrinfo in the destructor of our helper function, so if anything goes wrong
	(missconfiguration), RAII will ensure that any structs we have allocated so far will be freed
	(and at the end)
	
	All typedefs and helper structs placed on this .cpp file because we don't want to
	bloat the global namespace with them, not needed.

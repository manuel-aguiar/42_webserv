
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

		void							                    addListenAddress(const struct sockaddr* addr);         //helper
		const std::vector<const struct sockaddr*>&		    getListenAddresses() const;                              //getter

    The getter will be used by the BlockFinder to retrieve all the sockaddresses representing were
    the server will serve connections, to setup its internal data structures for matching client requests

    Internally, the pointers stored in the vector point to the relevant BindAddress::u_sockaddr, stored inside
    the ServerConfig

###############################################

Design considerations of ServerConfigDNSLookup.cpp

	Function divided into 4 stages:

	1. Do the DNS lookup for all the listeners in all server blocks
		a) as we go, we build a set of unique addrinfo structs and 
		b) we build a map of ALL listeners to their UNIQUE counterpart (the first that resolved to that addrinfo)
			(since two exact same addrinfo structs imply the same BindAddress, hence localhost being equal 127.0.0.1, same address)
	2. Create a vector (uniqueBind) of BindAddress structs, one for each unique addrinfo struct
		a) As we go, we also map the unique addrinfo to their BindAddress structs (uniqueBind)
	3. Fill the server blocks with their listeners, by mapping the listeners to their addrinfo and then to their BindAddress structs
		a) important step, each block may have duplicates themselves
			so we need to keep track, at each block, their own listener duplicates to avoid having the same input twice.
	4. Swap the uniqueBindAddress with the one on the ServerConfig, no need to copy

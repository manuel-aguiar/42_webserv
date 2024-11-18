

The main folder structure here is as follows:

ServerManager:
	- All things related to management of TCP, listening sockets, connections, IO, Events

ServerConfig:
	- Here is where the parsing of the configuration file is done
	In this folder we have class ServerBlocks:
		- The link between our configuration file and our websites. Each block
		will define domains, locations, ports, redirections, etc.
		The name comes from the {} blocks we expect on our configuration file where
		we define our website and interactions with the file system

Globals:
	- Quasi global variables that we will need throughout the program at all times and all places,
	such as log files, clocks


HTTP:
	- All things related to the HTTP protocol implementation: http connections/sessions/requests/responses

Cgi:
	- All things related to the CGI protocol implementation

Utils:
	- Files that are used throughout but are not actually globals. For instance: FileDescriptor class -> public static functions
		to interact with file descriptors that will probably be needed throughout

_Legacy, some old files

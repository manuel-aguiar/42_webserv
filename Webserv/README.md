

The main folder structure here is as follows:

ServerManager:
	- All things related to management of TCP, listening sockets, connections, IO, Events

ServerBlocks:
	- The link between our configuration file and our websites. Each block
	will define domains, locations, ports, redirections, etc.
	The name comes from the {} blocks we expect on our configuration file where
	we define our website and interactions with the file system

Globals:
	- Quasi global variables that we will need throughout the program at all times and all places,
	such as log files, clocks


HTTP:
	- All things related to HTTP: http connections/sessions/requests/responses

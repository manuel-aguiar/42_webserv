# Webserver Configuration

## Table of Contents
- [Webserver Configuration](#webserver-configuration)
	- [Table of Contents](#table-of-contents)
	- [Classes](#classes)
	- [Configuration File Rules](#configuration-file-rules)
	- [Configuration Directives](#configuration-directives)
		- [Program Directives](#program-directives)
		- [Server Directives](#server-directives)
		- [Location Directives](#location-directives)
	- [Example Configuration](#example-configuration)

## Classes
  - ServerConfig
    - The `ServerConfig` is responsible for parsing the configuration file and preparing the server blocks for the webserver.
  - ServerBlocks
    - Each `ServerBlocks` contains configuration data specific to a single server, including details such as the server's host, root directory, error page settings, locations etc...

## Configuration File Rules

- **Indentation**: Indentation is not required and is ignored during parsing.
- **Comments**: Lines starting with `#` are considered comments and are ignored during parsing.
- **Semicolons**: A semicolon at the end of a line is optional but may be used.
  
## Configuration Directives

The configuration file consists of several directives that define the behavior of the server. Below are the available directives:

### Program Directives

- **max_connections**  
  - Sets the maximum number of simultaneous connections per worker. The default value is `100`.

- **max_concurrent_cgi**  
  - Sets the maximum number of concurrent CGI processes. The default value is `10`.

- ~~**max_workers**~~

### Server Directives

- **listen**  
  - Specifies the IP address and port for the server in the format `[ip]:[port]`, or just `[port]`.
  - Default IP is `0.0.0.0`. There is no default port.
  
- **server_names**  
  - Accepts one or more server names (e.g., `example.com`, `www.example.com`).
  
- **root**  
  - Defines the root directory for the server (e.g., `./webroot/`).
  
- **client_body_size**  
  - Sets the maximum allowed size for the body of a request (e.g., `1M`, `1024`).

- **client_header_size**  
  - Sets the maximum allowed size for the header of a request (e.g., `1M`, `1024`).

- **error_pages**  
  - Specifies error pages for specific HTTP error codes. Use the format `[error-code]:[path]` (e.g., `404:/errors/404.html`).

- **location**
  - Defines how to process specific types of requests based on the URI. `location [path] { ... }`

### Location Directives

Defines rules for processing requests based on their URI.

- **type**
  - Set the location type. The options are:
    - normal:		Regular location, send and receive files.
    - redirection:	Redirects client to another location.
    - cgi:			Triggers a server side script to be executed.

- **methods**
  - Define what methods are accepted for the location.

- **autoindex**
  - Enables or disables directory listing.
  
- **root**
  - Sets the path that will be served at the defined location.

## Example Configuration

```plaintext
server {
    listen 192.168.1.53:8080;
    server_names webserv.com www.webserv.com;
    root ./webserv/;
    client_body_size 1M;
    client_header_size 1M;
    error_pages 404:./error/404.html 405:./error/405.html;
    max_connections 10;
    max_concurrent_cgi 3;
    
    location {
        type normal
    }
}
```


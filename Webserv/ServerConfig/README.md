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
  - ServerBlock
    - Each `ServerBlock` contains configuration data specific to a single server, including details such as the server's host, root directory, error page settings, locations etc...
  - ServerLocation 
    - Contains data for a `location`, defining how specific URI paths should be handled.

## Configuration File Rules

- **Indentation**: Indentation is not required and is ignored during parsing.
- **Comments**: Lines starting with `#` are considered comments and are ignored during parsing.
- **Semicolons**: A semicolon at the end of a line is optional but may be used.
- All directives must be on a new line, even if inside brackets.
  
## Configuration Directives

The configuration file consists of several directives that define the behavior of the server. Below are the available directives:

### Program Directives

- **max_connections**  
  - Sets the maximum number of simultaneous connections per worker. The default value is `100`.

- **max_concurrent_cgi**  
  - Sets the maximum number of concurrent CGI processes. The default value is `10`.

- **max_cgi_backlog**
  - Sets the maximum number of requests queued on the backlog. When this number is reached, new connections will be answered with an internal server error. The default value is `100`.

- **cgi**
  - Defines CGI interpreters for specific file extensions. Format: `extension:path`
  - Example: `cgi php:/usr/bin/php-cgi`

### Server Directives

- **listen**  
  - Specifies the IP address and port for the server in the format `[ip]:[port]`, or just `[port]`.
  - Default IP is `0.0.0.0`. There is no default port.
  
- **server_name**  
  - Accepts one or more server names (e.g., `example.com`, `www.example.com`).
  
- **root**  
  - Defines the root directory for the server (e.g., `./webroot/`).
  
- **client_body_size**  
  - Sets the maximum allowed size for the body of a request (e.g., `1M`, `1024`).

- **client_header_size**  
  - Sets the maximum allowed size for the header of a request (e.g., `1M`, `1024`).

- **error_pages**  
  - Specifies error pages for specific HTTP error codes. Format: `[error-code]:[path]`
  - Example: `404:/errors/404.html`

- **timeout_full_header**
  - Sets the timeout for receiving the complete header.

- **timeout_inter_send**
  - Sets the timeout between send operations.

- **timeout_inter_receive**
  - Sets the timeout between receive operations.

- **timeout_keep_alive**
  - Sets the keep-alive timeout for persistent connections.

### Location Directives

Defines rules for processing requests based on their URI.

- **path**
  - Sets the URI where the location settings will apply.

- **methods**
  - Define what HTTP methods are accepted (GET, POST, DELETE).

- **autoindex**
  - Enables (1) or disables (0) directory listing.
  
- **root**
  - Sets the path that will be served at the defined location.

- **index**
  - Specifies the default file to serve when requesting a directory.

- **return**
  - Configures a redirection. Format: `[code]:[url]`
  - Code must be between 300 and 399.

- **upload**
  - Enables (1) or disables (0) file uploads in this location.

- **cgi**
  - Configures CGI interpreters specific to this location.
  - Format: `extension:path`

## Example Configuration

```plaintext
max_connections 10;
max_concurrent_cgi 3;
max_cgi_backlog 100;

cgi php:/usr/bin/php-cgi;

server {
    listen 192.168.1.53:8080;
    server_name webserv.com www.webserv.com;
    root ./webserv/;
    client_body_size 1M;
    client_header_size 1M;
    error_pages 404:./error/404.html 405:./error/405.html;
    
    location {
        path /static/
        type regular
        methods GET POST
        autoindex 1
        root /var/www/static
    }

    location {
        path /uploads/
        type regular
        methods POST
        upload 1
        root /var/www/uploads
    }

    location {
        path /php/
        type cgi
        methods GET POST
        cgi php:/usr/bin/php-cgi
    }
}
```


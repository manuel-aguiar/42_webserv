# 42_webserv

A high-performance HTTP/1.1 web server implementation in C++98, developed as part of the 42 school curriculum.

## Features

- HTTP/1.1 protocol support
- Multiple server configurations
- Virtual host support
- Dynamic content handling through CGI
- File upload capabilities
- Directory listing
- Custom error pages
- Keep-alive connections
- Configurable timeout settings
- Event-driven architecture
- Support for multiple worker processes

## Building the Project

```bash
make        # Build in production mode
```

## Configuration

The server is configured through a configuration file that follows a Nginx-like syntax. Here's a basic example:

```nginx
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

### Configuration Options

#### Server Block
- `listen`: IP address and port for the server (default IP: 0.0.0.0)
- `server_name`: Domain names for virtual host routing
- `root`: Root directory for serving files
- `client_body_size`: Maximum size of client request body
- `client_header_size`: Maximum size of client request headers
- `error_pages`: Custom error page mappings

#### Location Block
- `path`: URL path to match
- `type`: Location type (regular, cgi, redirection)
- `methods`: Allowed HTTP methods
- `autoindex`: Enable/disable directory listing
- `root`: Location-specific root directory
- `upload`: Enable/disable file uploads
- `cgi`: CGI interpreter configuration

## Usage

```bash
./webserv <config_file>
```

## Default Settings

- Maximum concurrent CGI processes: 2
- Maximum connections: 2
- Maximum workers: 10
- Default client body size: 1MB
- Default client header size: 1MB
- Default timeout settings:
  - Full header timeout: 60 seconds
  - Inter-send timeout: 10 seconds
  - Inter-receive timeout: 10 seconds
  - Keep-alive timeout: 75 seconds

## Project Structure

- `Webserv/`: Core server implementation
  - `HttpModule/`: HTTP protocol implementation
  - `CgiModule/`: CGI processing
  - `ServerConfig/`: Configuration parsing
  - `Events/`: Event handling system
  - `Connections/`: Connection management
  - `GenericUtils/`: Utility functions
  - `StaticAssets/`: Default error pages and static content

## Requirements

- C++98 compatible compiler
- Linux/Unix-based operating system
- Make build system

## Authors

- [Manuel Aguiar](https://github.com/manuel-aguiar)
- [Rafael Vaz](https://github.com/rafaelva-z)
- [Rajh Phuyal](https://github.com/rajh-phuyal)

## License

This project is part of the 42 school curriculum.
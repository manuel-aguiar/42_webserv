# Webserv Architecture Overview

## Introduction

This document provides an overview of the architecture of our Webserv HTTP server implementation. Webserv is designed to be a lightweight, efficient HTTP server similar to Nginx, implemented in C++98.

## Core Components

### 1. Connection Management

The connection management subsystem handles incoming connections, managing sockets, and maintaining the connection lifecycle. Key components include:

- **Listener**: Responsible for accepting new incoming connections on configured ports
- **Connection Pool**: Manages active connections and their states
- **Event System**: Uses select/poll/epoll for non-blocking I/O operations

### 2. HTTP Module

The HTTP module processes HTTP requests and generates appropriate responses:

- **Request Parser**: Parses incoming HTTP request headers and bodies
- **Response Generator**: Creates HTTP responses based on request processing
- **Header Management**: Handles HTTP headers for both requests and responses

### 3. Configuration System

The configuration system allows customization of server behavior through configuration files:

- **Parser**: Reads and parses configuration files
- **Validator**: Ensures configuration validity
- **Virtual Servers**: Supports multiple server configurations on different ports/hostnames

### 4. File System Interface

The file system interface provides access to static files and directories:

- **File Reader**: Efficiently reads files to serve as responses
- **Directory Listing**: Generates HTML directory listings when enabled
- **File Type Detection**: Identifies MIME types for served files

### 5. CGI Module

The CGI (Common Gateway Interface) module allows execution of scripts:

- **Process Management**: Creates and manages child processes for CGI execution
- **Environment Setup**: Prepares environment variables for CGI scripts
- **I/O Handling**: Manages input/output between the server and CGI scripts

## Request Processing Flow

1. The Listener accepts a new connection and creates a Connection object
2. The Event System monitors the connection for activity
3. When data is available, the HTTP Parser processes the request
4. Based on the request, appropriate handler is selected (file, directory, CGI, etc.)
5. The handler processes the request and generates a response
6. The response is sent back to the client
7. The connection is either kept alive or closed based on the HTTP headers

## Directory Listing Feature

The directory listing feature is implemented as follows:

1. When a request targets a directory, check if an index file exists
2. If no index file exists and auto_index is enabled, scan the directory contents
3. Generate an HTML representation of the directory contents
4. Send the HTML as the response with appropriate Content-Type header

## Performance Considerations

- Non-blocking I/O for handling multiple connections
- Connection timeouts to prevent resource exhaustion
- Buffer management to optimize memory usage
- Configurable worker processes for multi-core systems

## Configuration Example

```
server {
    listen 8080
    server_name example.com
    root ./webroot

    location {
        path /
        index index.html
        auto_index 1
    }

    location {
        path /api
        methods GET POST
        auto_index 0
    }
}
```

## Conclusion

Webserv implements a robust HTTP server architecture that balances simplicity with functionality. The modular design allows for easy extension and maintenance, while adhering to HTTP standards for compatibility with web clients.
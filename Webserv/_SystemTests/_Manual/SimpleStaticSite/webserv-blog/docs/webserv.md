# Webserv Project

## Overview

Webserv is a project focused on creating a simple HTTP server from scratch. The goal is to implement a server that can handle HTTP requests and serve static content, as well as handle CGI scripts.

## Key Features

- HTTP/1.1 protocol implementation
- Static file serving
- Directory listing
- CGI support
- Multiple virtual servers
- Custom error pages

## Configuration

The server is configured using a configuration file similar to Nginx. The configuration file uses a simple syntax to define:

- Server blocks
- Listen directives
- Location blocks
- Directory listing options
- Error pages

## Directory Listing

One of the key features of the webserv project is directory listing. When enabled using the `auto_index 1` directive, the server will generate a list of files and directories when a user tries to access a directory without an index file.

Example configuration:

```
location {
    path /docs/
    root ./StaticSite/docs
    methods get
    auto_index 1
}
```

## Error Handling

The server can be configured to use custom error pages for different HTTP status codes. For example:

```
error_pages 404:./error/404.html 500:./error/500.html
```

This allows for a better user experience when errors occur.
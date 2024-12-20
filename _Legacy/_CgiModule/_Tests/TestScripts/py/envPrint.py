#!/usr/bin/env python3
import os

def main():
    env_vars = [
        "AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERFACE",
        "PATH_INFO", "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR",
        "REMOTE_HOST", "REMOTE_IDENT", "REMOTE_USER", "REQUEST_METHOD",
        "SCRIPT_NAME", "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL",
        "SERVER_SOFTWARE"
    ]

    print("Content-Type: text/plain\n")
    for var in env_vars:
        value = os.getenv(var, "<not set>")
        print(f"{var}: {value}")

if __name__ == "__main__":
    main()
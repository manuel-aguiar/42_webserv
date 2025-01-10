#!/usr/bin/env php
<?php

// List of CGI environment variables
$env_vars = [
    "AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERFACE",
    "PATH_INFO", "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR",
    "REMOTE_HOST", "REMOTE_IDENT", "REMOTE_USER", "REQUEST_METHOD",
    "SCRIPT_NAME", "SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL",
    "SERVER_SOFTWARE", "CUSTOM_ENTRY1", "CUSTOM_ENTRY2", "CUSTOM_ENTRY3"
];

// Fetch and print each environment variable using getenv()
foreach ($env_vars as $var) {
    $value = getenv($var);
    echo "$var: " . ($value !== false ? $value : "<not set>") . "\n";
}

?>
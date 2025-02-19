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

header("Connection: close");  // Invalid header
header("Transfer-Encoding: chunkedasfasfasfasf");  // Invalid header
echo "Content-Type: text/html; charset=UTF-8";  // Valid header

echo " wtf : ";
header("asgasgasgasgasgasgasgasg: close");  // Invalid header
header("Status: 404 Not Found");  // Invalid header
header("Status: 404 Not Found");  // Invalid header
header("Status: 404 Not Found");  // Invalid header
header("Status: 404 Not Found");  // Invalid header
echo " wtf : ";
echo " wtf : ";

// Fetch and print each environment variable using getenv()
foreach ($env_vars as $var) {
    $value = getenv($var);
    echo "$var: " . ($value !== false ? $value : "<not set>") . "\n";
}

echo " wtf : ";

?>
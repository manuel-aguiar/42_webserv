#!/usr/bin/php
<?php
// Required header for CGI scripts
echo ("tretas: 1000\n");
echo ("content-Type: text/html;charset=UTF-8\n");
echo "\n";
echo "<!DOCTYPE html>\n";
echo "<html lang='en'>\n";
echo "<head><meta charset='UTF-8'><title>CGI Test</title></head>\n";
echo "<body>\n";
echo "<h1>Hello from PHP CGI!</h1>\n";
echo "<p>This is a simple PHP script executed via CGI.</p>\n";
echo "<pre>\n";
echo getenv("SERVER_NAME");
echo "</pre>\n";
echo "</body></html>\n";
?>
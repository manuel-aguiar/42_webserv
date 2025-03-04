#!/usr/bin/php
<?php
// Required header for CGI scripts
//echo ("Content-type: text/html;charset=UTF-8\n\n");
header("tretas: 1000");
header("status: 200");
header("status: 200");
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
#!/usr/bin/php
<?php
// Required header for CGI scripts
header("Content-Type: text/html");

echo "<!DOCTYPE html>";
echo "<html lang='en'>";
echo "<head><meta charset='UTF-8'><title>CGI Test</title></head>";
echo "<body>";
echo "<h1>Hello from PHP CGI!</h1>";
echo "<p>This is a simple PHP script executed via CGI.</p>";
echo "<pre>";
echo getenv("SERVER_NAME");
echo "</pre>";
echo "</body></html>";
?>
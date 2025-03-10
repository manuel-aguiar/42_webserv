#!/usr/bin/python3

import os

# Required header for CGI scripts
print("Content-type: text/html;charset=UTF-8\n")

print("<!DOCTYPE html>")
print("<html lang='en'>")
print("<head><meta charset='UTF-8'><title>CGI Test</title></head>")
print("<body>")
print("<h1>Hello from Python CGI!</h1>")
print("<p>This is a simple Python script executed via CGI.</p>")
print("<pre>")
print(os.getenv("SERVER_NAME", "Unknown Server"))
print("</pre>")
print("</body></html>")

for i in range(10000000000000):
	continue 
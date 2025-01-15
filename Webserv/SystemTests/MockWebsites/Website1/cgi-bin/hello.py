#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
 **************************************************************************
         this was chatgpt, btw
                                                        :::      ::::::::
   hello.py                                           :+:      :+:    :+:
                                                    +:+ +:+         +:+
   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+
                                                +#+#+#+#+#+   +#+
   Created: 2024/10/05 12:37:49 by mmaria-d          #+#    #+#
   Updated: 2024/10/05 12:37:49 by mmaria-d         ###   ########.fr

 **************************************************************************
"""

# Import the CGI module and sys for reading input
import os

# Content to be output
html_content = """
<html>
  <body>
    <h1>Hello, World from Python CGI!</h1>
    <p>This is a simple CGI response.</p>
  </body>
</html>
"""

# Calculate content length
content_length = len(html_content)

# Output the HTTP headers
print("Content-Type: text/html")
print(f"Content-Length: {content_length}")
print()  # Blank line separates headers from content

# Output the actual content
print(html_content)

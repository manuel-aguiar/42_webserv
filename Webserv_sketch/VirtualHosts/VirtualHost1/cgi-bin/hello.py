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
import cgi
import sys

# Output the HTTP header
print("Content-Type: text/html\n")

# Simple HTML output
print("<html><body>")
print("<h1>Hello, World from Python-CGI!</h1>")
print("</body></html>")
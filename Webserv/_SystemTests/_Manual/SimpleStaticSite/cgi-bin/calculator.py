#!/usr/bin/env python3
import cgi
import cgitb
import sys
import os
import json
from datetime import datetime

# Enable CGI error tracking for debugging
cgitb.enable()

def main():
    # Print HTTP headers
    print("Content-Type: text/html\r\n\r")

    # Get form data if any
    form = cgi.FieldStorage()

    # Initialize variables
    operation = form.getvalue("operation", "add")
    num1 = form.getvalue("num1", "5")
    num2 = form.getvalue("num2", "3")
    result = None
    error = None

    # Handle form submission if parameters exist
    if "num1" in form and "num2" in form:
        try:
            num1 = float(num1)
            num2 = float(num2)

            if operation == "add":
                result = num1 + num2
                operation_text = "Addition"
            elif operation == "subtract":
                result = num1 - num2
                operation_text = "Subtraction"
            elif operation == "multiply":
                result = num1 * num2
                operation_text = "Multiplication"
            elif operation == "divide":
                if num2 == 0:
                    raise ZeroDivisionError("Cannot divide by zero")
                result = num1 / num2
                operation_text = "Division"
            elif operation == "power":
                result = num1 ** num2
                operation_text = "Power"
            else:
                raise ValueError("Invalid operation")

        except ZeroDivisionError as e:
            error = str(e)
        except ValueError as e:
            error = "Please enter valid numbers"
        except Exception as e:
            error = f"An error occurred: {str(e)}"
    else:
        # Default for first load
        operation_text = "Addition"

    # Format result
    if result is not None:
        if result.is_integer():
            result = int(result)
        formatted_result = f"{result:,}"
    else:
        formatted_result = "N/A"

    # Build HTML response
    print(f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>CGI Calculator - 42 Webserv</title>
        <style>
            body {{
                font-family: Arial, sans-serif;
                line-height: 1.6;
                max-width: 800px;
                margin: 0 auto;
                padding: 20px;
                color: #333;
            }}
            h1, h2 {{
                color: #2c3e50;
            }}
            .calculator-container {{
                background-color: #f5f5f5;
                border-radius: 8px;
                padding: 20px;
                margin-bottom: 20px;
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }}
            .form-group {{
                margin-bottom: 15px;
            }}
            label {{
                display: block;
                margin-bottom: 5px;
                font-weight: bold;
            }}
            input, select {{
                width: 100%;
                padding: 8px;
                border: 1px solid #ddd;
                border-radius: 4px;
                box-sizing: border-box;
            }}
            button {{
                background-color: #3498db;
                color: white;
                border: none;
                padding: 10px 15px;
                border-radius: 4px;
                cursor: pointer;
                font-size: 16px;
            }}
            button:hover {{
                background-color: #2980b9;
            }}
            .result {{
                margin-top: 20px;
                padding: 15px;
                background-color: #e8f4fc;
                border-radius: 4px;
                font-size: 18px;
            }}
            .error {{
                color: #e74c3c;
                background-color: #fadbd8;
                padding: 10px;
                border-radius: 4px;
                margin-top: 10px;
            }}
            .server-info {{
                margin-top: 30px;
                font-size: 0.8rem;
                color: #7f8c8d;
            }}
            .server-info code {{
                background-color: #f0f0f0;
                padding: 2px 4px;
                border-radius: 3px;
            }}
            .nav {{
                margin-top: 20px;
            }}
            .nav a {{
                text-decoration: none;
                color: #3498db;
            }}
            .nav a:hover {{
                text-decoration: underline;
            }}
        </style>
    </head>
    <body>
        <h1>CGI Calculator</h1>
        <p>This is a demonstration of CGI script processing in Webserv. The calculator below is implemented as a Python CGI script.</p>

        <div class="calculator-container">
            <h2>Perform a Calculation</h2>
            <form method="GET" action="/cgi-bin/calculator.py">
                <div class="form-group">
                    <label for="operation">Select Operation:</label>
                    <select id="operation" name="operation">
                        <option value="add" {"selected" if operation == "add" else ""}>Addition (+)</option>
                        <option value="subtract" {"selected" if operation == "subtract" else ""}>Subtraction (-)</option>
                        <option value="multiply" {"selected" if operation == "multiply" else ""}>Multiplication (×)</option>
                        <option value="divide" {"selected" if operation == "divide" else ""}>Division (÷)</option>
                        <option value="power" {"selected" if operation == "power" else ""}>Power (^)</option>
                    </select>
                </div>

                <div class="form-group">
                    <label for="num1">First Number:</label>
                    <input type="text" id="num1" name="num1" value="{num1}" required>
                </div>

                <div class="form-group">
                    <label for="num2">Second Number:</label>
                    <input type="text" id="num2" name="num2" value="{num2}" required>
                </div>

                <button type="submit">Calculate</button>
            </form>

            {"<div class='error'>" + error + "</div>" if error else ""}

            {"<div class='result'>" + operation_text + " result: <strong>" + formatted_result + "</strong></div>" if result is not None else ""}
        </div>

        <div class="server-info">
            <h3>Server Information:</h3>
            <p>CGI Version: Python {sys.version.split()[0]}</p>
            <p>Server Time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
            <p>Server Software: {os.environ.get('SERVER_SOFTWARE', 'Unknown')}</p>
            <p>Remote Address: {os.environ.get('REMOTE_ADDR', 'Unknown')}</p>
            <p>Query String: {os.environ.get('QUERY_STRING', 'None')}</p>
            <p>Request Method: {os.environ.get('REQUEST_METHOD', 'Unknown')}</p>
        </div>

        <div class="nav">
            <a href="/">Back to Home</a>
        </div>
    </body>
    </html>
    """)

if __name__ == "__main__":
    main()
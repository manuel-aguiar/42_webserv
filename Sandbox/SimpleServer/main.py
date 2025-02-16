#!/usr/bin/env python3

import http.server
import socketserver

PORT = 5674

# HTML page with forms and a button to send DELETE via JS fetch
HTML_PAGE = b"""\
<html>
<head><title>HTTP Method Test</title></head>
<body>
  <h1>Test HTTP Methods</h1>

  <h2>GET request</h2>
  <form action="/get-test" method="GET">
    <input type="text" name="q" placeholder="Query">
    <button type="submit">Send GET</button>
  </form>

  <hr>

  <h2>POST request (x-www-form-urlencoded)</h2>
  <form action="/post-test" method="POST" enctype="application/x-www-form-urlencoded">
    <input type="text" name="username" placeholder="Username">
    <button type="submit">Send POST</button>
  </form>

  <hr>

  <h2>POST request (file upload)</h2>
  <form action="/upload" method="POST" enctype="multipart/form-data">
    <input type="file" name="myfile">
    <button type="submit">Upload</button>
  </form>

  <hr>

  <h2>DELETE request (JavaScript)</h2>
  <!-- Browsers won't send DELETE from a plain HTML form, so we use fetch. -->
  <button onclick="
    fetch('/delete-test', { method: 'DELETE' })
      .then(res => res.text())
      .then(txt => alert('DELETE response: ' + txt))
      .catch(err => console.error(err));
  ">
    Send DELETE
  </button>
</body>
</html>
"""


class SimpleHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        """
        Handle GET requests. We'll serve our HTML at path '/'
        and for all other GETs, just log raw request info.
        """
        if self.path == '/':
            # Serve the HTML form
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', str(len(HTML_PAGE)))
            self.end_headers()
            self.wfile.write(HTML_PAGE)
        else:
            # Log raw request info:
            self._log_request_info()
            # Send a simple response
            self.send_response(200)
            self.send_header('Content-Type', 'text/plain')
            self.end_headers()
            self.wfile.write(b"This was a GET request.\nCheck server console for details.\n")

    def do_POST(self):
        """
        Handle POST requests. Log the raw request (headers + body)
        and respond with a simple text. This covers both x-www-form-urlencoded
        and multipart/form-data (file uploads).
        """
        self._log_request_info()
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
        self.wfile.write(b"This was a POST request.\nCheck server console for details.\n")

    def do_DELETE(self):
        """
        Handle DELETE requests (triggered via JavaScript fetch in our HTML).
        """
        self._log_request_info()
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
        self.wfile.write(b"This was a DELETE request.\nCheck server console for details.\n")

    def _log_request_info(self):
        """
        Utility: print out the request method, path, headers, and raw body to the console.
        """
        print("----- REQUEST START -----")
        print(f"Method: {self.command}")
        print(f"Path: {self.path}")
        print("Headers:")
        for k, v in self.headers.items():
            print(f"    {k}: {v}")

        # Read body if Content-Length is present
        length = self.headers.get('Content-Length')
        if length is not None:
            length = int(length)
            body_bytes = self.rfile.read(length)
            print("\nBody (raw bytes):")
            print(body_bytes)
            try:
                print("\nBody (decoded):")
                print(body_bytes.decode("utf-8", errors="replace"))
            except Exception as e:
                print(f"[Could not decode body: {e}]")
        else:
            print("\nNo Content-Length, body may be empty or chunked.")
            # NOTE: For chunked data, you'd have to handle chunk parsing.
            # The built-in BaseHTTPRequestHandler doesn't automatically do that.

        print("----- REQUEST END -------\n")


def run_server():
    with socketserver.TCPServer(("", PORT), SimpleHandler) as httpd:
        print(f"Serving at port {PORT}, open http://localhost:{PORT}/")
        httpd.serve_forever()


if __name__ == "__main__":
    run_server()

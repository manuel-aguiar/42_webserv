import socket

def start_server():
    host = "localhost"
    port = 8080
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(1)

    print(f"Listening on {host}:{port}...")
    while True:
        conn, addr = server.accept()
        print(f"Connection from {addr}")
        response = """HTTP/1.1 200 OK
Transfer-Encoding: chunked

0000003\r\nhey\r\n0\r\n\r\n"""
        conn.sendall(response.encode())
        conn.close()

if __name__ == "__main__":
    start_server()
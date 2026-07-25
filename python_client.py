import socket

ESP32_IP = "192.168.1.100"
PORT = 5000
TIMEOUT_SECONDS = 10

def main() -> None:
    try:
        with socket.create_connection((ESP32_IP, PORT), timeout=TIMEOUT_SECONDS) as sock:
            welcome = sock.recv(1024).decode(errors="replace").strip()
            print("ESP32:", welcome)

            while True:
                message = input("You: ").strip()
                if message.lower() in {"exit", "quit"}:
                    break

                sock.sendall((message + "\n").encode())
                reply = sock.recv(1024).decode(errors="replace").strip()
                print("ESP32:", reply)

    except (OSError, ConnectionError) as error:
        print(f"Connection failed: {error}")

if __name__ == "__main__":
    main()

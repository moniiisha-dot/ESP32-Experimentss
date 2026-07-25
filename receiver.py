import socket
import struct
from pathlib import Path

HOST = "0.0.0.0"
PORT = 5000
OUTPUT_DIRECTORY = Path("received")
CHUNK_SIZE = 64 * 1024

def receive_exact(connection: socket.socket, size: int) -> bytes:
    data = bytearray()

    while len(data) < size:
        chunk = connection.recv(size - len(data))
        if not chunk:
            raise ConnectionError("Connection closed before all data was received.")
        data.extend(chunk)

    return bytes(data)

def safe_filename(name: str) -> str:
    return Path(name).name or "received_file.bin"

def main() -> None:
    OUTPUT_DIRECTORY.mkdir(exist_ok=True)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind((HOST, PORT))
        server.listen(1)

        print(f"Waiting for sender on port {PORT}...")

        connection, address = server.accept()

        with connection:
            print("Connected:", address)

            name_length = struct.unpack("!H", receive_exact(connection, 2))[0]
            filename = safe_filename(
                receive_exact(connection, name_length).decode("utf-8", errors="replace")
            )
            file_size = struct.unpack("!Q", receive_exact(connection, 8))[0]

            destination = OUTPUT_DIRECTORY / filename
            remaining = file_size

            with destination.open("wb") as output:
                while remaining:
                    chunk = connection.recv(min(CHUNK_SIZE, remaining))
                    if not chunk:
                        raise ConnectionError("Transfer ended early.")
                    output.write(chunk)
                    remaining -= len(chunk)

            print(f"Saved: {destination} ({file_size} bytes)")

if __name__ == "__main__":
    try:
        main()
    except (OSError, ConnectionError, ValueError) as error:
        print(f"Receiver stopped: {error}")

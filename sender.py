import socket
import struct
from pathlib import Path

RECEIVER_IP = "172.20.10.5"
PORT = 5000
FILE_PATH = Path("image.jpg")
CHUNK_SIZE = 64 * 1024

def main() -> None:
    if not FILE_PATH.is_file():
        raise FileNotFoundError(f"File not found: {FILE_PATH}")

    file_size = FILE_PATH.stat().st_size
    file_name = FILE_PATH.name.encode("utf-8")

    with socket.create_connection((RECEIVER_IP, PORT), timeout=15) as sock:
        sock.sendall(struct.pack("!H", len(file_name)))
        sock.sendall(file_name)
        sock.sendall(struct.pack("!Q", file_size))

        with FILE_PATH.open("rb") as file:
            while chunk := file.read(CHUNK_SIZE):
                sock.sendall(chunk)

    print(f"Transfer complete: {FILE_PATH.name} ({file_size} bytes)")

if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError) as error:
        print(f"Transfer failed: {error}")

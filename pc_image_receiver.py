import struct
import time
from pathlib import Path

import serial

PORT = "COM7"
BAUD_RATE = 115200
OUTPUT_DIRECTORY = Path("received_images")
MAX_IMAGE_SIZE = 200_000

def read_exact(connection: serial.Serial, size: int) -> bytes:
    data = bytearray()

    while len(data) < size:
        chunk = connection.read(size - len(data))
        if not chunk:
            raise TimeoutError(f"Timed out after receiving {len(data)} of {size} bytes.")
        data.extend(chunk)

    return bytes(data)

def main() -> None:
    OUTPUT_DIRECTORY.mkdir(exist_ok=True)

    with serial.Serial(PORT, BAUD_RATE, timeout=5) as connection:
        connection.reset_input_buffer()
        print(f"Listening on {PORT}...")

        while True:
            # This receiver expects the computer-connected board to forward
            # a four-byte image length followed by JPEG bytes.
            length_bytes = read_exact(connection, 4)
            image_size = struct.unpack("<I", length_bytes)[0]

            if image_size <= 0 or image_size > MAX_IMAGE_SIZE:
                print(f"Ignoring invalid image size: {image_size}")
                continue

            image_data = read_exact(connection, image_size)
            filename = OUTPUT_DIRECTORY / f"image_{int(time.time())}.jpg"
            filename.write_bytes(image_data)
            print(f"Saved {filename} ({image_size} bytes)")

if __name__ == "__main__":
    try:
        main()
    except (serial.SerialException, TimeoutError, OSError) as error:
        print(f"Receiver stopped: {error}")

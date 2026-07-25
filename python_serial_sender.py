import time
import serial

PORT = "COM7"
BAUD_RATE = 115200

def send_command(connection: serial.Serial, command: str) -> None:
    connection.write((command.strip() + "\n").encode("utf-8"))
    connection.flush()
    print("Sent:", command)

def main() -> None:
    commands = [
        "WAIT",
        "HELLO:Monisha",
        "BLINK",
        "LEFT",
        "RIGHT",
        "UP",
        "DOWN",
        "SMILE",
        "VERIFIED:Monisha",
        "REGISTERED",
        "FAILED",
    ]

    try:
        with serial.Serial(PORT, BAUD_RATE, timeout=1) as connection:
            time.sleep(2)

            for command in commands:
                send_command(connection, command)
                time.sleep(2)

    except serial.SerialException as error:
        print(f"Serial connection failed: {error}")

if __name__ == "__main__":
    main()

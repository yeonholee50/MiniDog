import serial
import time

# Setup serial communication with the Arduino
arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)  # Adjust the port to match your setup
time.sleep(2)  # Give some time for the serial connection to establish

def get_distance():
    # Trigger the Arduino to send distance data
    arduino.write(b'D')  # Assume 'D' triggers distance reading on Arduino
    distance = arduino.readline().decode('utf-8').strip()
    return float(distance)

def move_robot(command):
    arduino.write(command.encode('utf-8'))
    time.sleep(0.1)

def maintain_distance(target_distance):
    while True:
        distance = get_distance()
        print(f"Distance: {distance} cm")

        if distance > target_distance + 5:
            move_robot('F')  # Move forward if too far
        elif distance < target_distance - 5:
            move_robot('B')  # Move backward if too close
        else:
            move_robot('S')  # Stop if within range

        time.sleep(0.5)

if __name__ == "__main__":
    try:
        maintain_distance(15)  # Target distance of 15 cm
    except KeyboardInterrupt:
        arduino.close()
        print("Program terminated.")

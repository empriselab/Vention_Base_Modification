import pygame
import serial
import time

# === CONFIGURATION ===
SERIAL_PORT = '/dev/ttyACM0'  # <-- Replace with your Arduino port
BAUD_RATE = 9600
DEADBAND = 0.2
COMMAND_INTERVAL = 0.01  # seconds

# === INIT SERIAL ===
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
time.sleep(2)  # Wait for Arduino to reset

# === INIT PYGAME ===
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    raise Exception("No game controller connected!")

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Controller: {joystick.get_name()}")

last_cmd = ''

def send_command(cmd):
    global last_cmd
    if cmd != last_cmd:
        ser.write(cmd.encode())
        print(f"[→] Sent command: '{cmd}'")
        last_cmd = cmd

# === MAIN LOOP ===
try:
    while True:
        pygame.event.pump()

        x_axis = joystick.get_axis(0)  # Left joystick X
        y_axis = joystick.get_axis(1)  # Left joystick Y

        cmd = 's'  # Default: stop

        # Apply deadband
        if abs(y_axis) > DEADBAND:
            cmd = 'f' if y_axis < 0 else 'b'
        elif abs(x_axis) > DEADBAND:
            cmd = 'l' if x_axis < 0 else 'r'

        send_command(cmd)
        time.sleep(COMMAND_INTERVAL)

except KeyboardInterrupt:
    send_command('s')
    print("\n[!] Stopped. Exiting...")
finally:
    joystick.quit()
    pygame.quit()
    ser.close()

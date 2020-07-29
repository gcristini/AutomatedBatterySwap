from CustomSerial import CustomSerial
from serial import SerialException
import sys

class SerialRelay(object):
    def __init__(self, port=None, baudrate=None):
        self._port = port
        self._baudrate = baudrate
        self._serial = None

        pass

    def init(self):
        """"""
        try:
            self._serial = CustomSerial(port=self._port,
                                        baudrate=self._baudrate)
        except SerialException:
            print("- {port} not found. Available ports: \n  {list}".format(port=self._port,
                                                                           list=CustomSerial.list_available_serial(100)))
            input(" Press a key to exit...")
            sys.exit()
        else:
            self._serial.serial_init()
            print("- Relays connected")
        pass

    def drive_relay(self, cmd):
        """"""
        # Concatenate command
        fcmd = "{cmd}\r".format(cmd=cmd)

        # Send command to relay
        self._serial.serial_write(fcmd)

        pass

    def close(self):
        """"""
        self._serial.serial_close()
        pass
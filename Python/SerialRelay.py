from CustomSerial import CustomSerial


class SerialRelay(object):
    def __init__(self, port=None, baudrate=None):
        self._port = port
        self._baudrate = baudrate
        self._serial = None

        pass

    def init(self):
        """"""
        self._serial = CustomSerial(port=self._port,
                                    baudrate=self._baudrate)
        self._serial.serial_init()

        print("- relays connected")

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
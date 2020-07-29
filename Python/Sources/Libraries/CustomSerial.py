# Created by gcristini at 19/10/2018

import serial
from serial import SerialException


class CustomSerial(serial.Serial):

    def __init__(self, **kwargs):
        """ Initialize (???) """
        super(CustomSerial, self).__init__(**kwargs)
        self._dataRX = ""
        self._bytes_available_rx = 0

    def serial_init(self):
        """ Open serial port, if closed """
        if self.is_open:
            self.close()
            self.open()
        else:
            None  # do nothing

    def serial_write(self, data):
        """ Use 'write' method of Serial class enconding data in UTF-8"""
        self.write(data.encode())
        return

    def serial_read(self):
        """ Use 'read' method of Serial class enconding data in UTF-8"""
        # TODO controllare codifica!
        self._dataRX = self.read(self._bytes_available_rx).decode('utf-8')
        return self._dataRX

    def serial_close(self):
        self.close()
        return

    @staticmethod
    def list_available_serial(max_num):
        serial_list = []
        port=""

        for num in range(1, max_num):
            port="COM{num}".format(num=num)
            try:
                serial.Serial(port=port)
            except SerialException:
                pass
            else:
                serial_list.append(port)
        return serial_list

    @property
    def bytes_available_rx(self):
        """ Check available bytes of data """
        self._bytes_available_rx = self.inWaiting()
        return self._bytes_available_rx


if __name__ == '__main__':
    print (CustomSerial.list_available_serial(100))

    test_serial = CustomSerial(port="COM16", baudrate=115200)
    test_serial.serial_init()

    while True:
        if test_serial.bytes_available_rx:
            x = test_serial.serial_read()
            print(type(x))

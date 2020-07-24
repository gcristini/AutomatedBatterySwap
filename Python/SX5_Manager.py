import os
import subprocess
import signal
from ParseXml import XmlDictConfig
from xml.etree import ElementTree
from adb_shell.adb_device import AdbDeviceTcp
from Timer import Timer
import sys

class ADB_Error(Exception):
    # TODO
    pass

class SX5_Manager(object):
    """ """
    # ************************************************* #
    # **************** Private Methods **************** #
    # ************************************************* #
    def __init__(self):
        """ Constructor"""
        self._sx5_shell_values = {
            'SupercapVoltage_mV': int,
            'CapokFlag': bool
        }

        self._sx5_config_dict = dict
        self._sx5_device = AdbDeviceTcp

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def _parse_config_file(self):
        """"""
        self._sx5_config_dict = XmlDictConfig(ElementTree.parse('Config.xml').getroot())['LoopTest']

        pass

    def _adb_init(self):
        """"""
        count = 0
        res = -1
        num_of_try = 30

        # Start Timer
        timer = Timer()
        timer.start()

        while (count < num_of_try):
            if timer.elapsed_time_s(2) >= 1:
                print("- Waiting for device " + "."*count, end='\r')
                res = subprocess.run("adb devices", text=True, capture_output=True).stdout.find(self._sx5_config_dict['SX5']['device'])
                if (res != -1):
                    timer.stop()
                    break
                else:
                    timer.reset()

                count += 1
        if res == -1:
            sys.stdout.write("\033[K")
            print("No Device Found")
            raise ADB_Error
        else:
            sys.stdout.write("\033[K")
            self._sx5_device = AdbDeviceTcp(host=self._sx5_config_dict['SX5']['ip'],
                                            port=int(self._sx5_config_dict['SX5']['port']),
                                            default_transport_timeout_s=9.)
        pass

    def _adb_tcp_connect(self):
        """"""
        try:
            self._sx5_device.connect(auth_timeout_s=0.1)
            print("- SX5 Connected")
        except:
            print("Timeout expired: check if device is on and if the IP is correct")
            raise
        pass

    def _adb_tcp_disconnect(self):
        """"""
        self._sx5_device.close()
        pass

    # ---------------------------------------------------------------- #
    # ------------------------ Public Methods ------------------------ #
    # ---------------------------------------------------------------- #

    def init(self):
        """ Initialize class and connect SX5 to ADB over TCP-IP protocol """
        # Parse config file
        self._parse_config_file()

        # Initialize adb connection
        try:
            self._adb_init()
        except ADB_Error:
            sys.exit()
        try:
            self._adb_tcp_connect()
        except:
            subprocess.run("adb disconnect")
            subprocess.run("adb tcpip {port}".format(port=int(self._sx5_config_dict['SX5']['port'])))
            #self._adb_init()
            self._adb_tcp_connect()
        pass

    def read_supercap_voltage_mV(self):
        """"""
        self._sx5_shell_values['SupercapVoltage_mV'] = int(self._sx5_device.shell("cat /sys/bus/platform/devices/vendor:supercap/voltage").strip(" mV\n"))
        pass

    def read_capok_flag(self):
        self._sx5_shell_values['CapokFlag'] = bool(int(self._sx5_device.shell(" cat /sys/bus/platform/devices/vendor:supercap/capok").strip("\n")))
        pass

    @property
    def supercap_voltage_mV(self):
        return self._sx5_shell_values['SupercapVoltage_mV']

    @property
    def capok_flag(self):
        return self._sx5_shell_values['CapokFlag']


if __name__ == "__main__":
    import time
    from Timer import Timer
    test = SX5_Manager()
    test.init()

    ct = Timer()
    ct.start()

    generalTimer = Timer()
    generalTimer.start()
    counter = 0
    while generalTimer.elapsed_time_s(1) < 10:
        test.read_supercap_voltage_mV()
        counter +=1

    print ("commands per seconds:" + str(counter/10))








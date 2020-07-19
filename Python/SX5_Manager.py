import os
import subprocess
import signal
from ParseXml import XmlDictConfig
from xml.etree import ElementTree
from adb_shell.adb_device import AdbDeviceTcp

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
        self._supercap_voltage_mV = str
        self._sx5_config_dict = dict
        self._sx5_device = AdbDeviceTcp

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def _parse_config_file(self):
        """"""
        self._sx5_config_dict = XmlDictConfig(ElementTree.parse('LT_Config.xml').getroot())

        pass

    def _adb_init(self):
        """"""
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
            self._adb_tcp_connect()
        except :
            subprocess.run("adb disconnect")
            subprocess.run("adb tcpip {port}".format(port=int(self._sx5_config_dict['SX5']['port'])))
            self._adb_init()
            self._adb_tcp_connect()

        pass

    def read_supercap_voltage_mV(self):
        """"""
        self._supercap_voltage_mV = int(self._sx5_device.shell("cat /sys/bus/platform/devices/vendor:supercap/voltage").strip(" mV\n"))

        return self._supercap_voltage_mV

    @property
    def supercap_voltage_mV(self):
        return self._supercap_voltage_mV


if __name__ == "__main__":
    import time
    from Timer import Timer
    test = SX5_Manager()
    test.init()

    ct = Timer()
    ct.start()

    generalTimer = Timer()
    generalTimer.start()

    while True:
        if ct.elapsed_time_ms() > 50:
            test.read_supercap_voltage_mV()
            print(generalTimer.elapsed_time_ms(), test.supercap_voltage_mV)
            ct.reset()






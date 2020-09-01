import subprocess

from Libraries.ParseXml import XmlDictConfig
from xml.etree import ElementTree
from adb_shell.adb_device import AdbDeviceTcp
from adb_shell import exceptions
from Libraries.Timer import Timer
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
        self._sx5_shell_values_dict = {
            'SupercapVoltage_mV': {
                'command': 'cat /sys/bus/platform/devices/vendor:supercap/voltage',
                'value:': ""
            },
            'CapokFlag': {
                'command': 'cat /sys/bus/platform/devices/vendor:supercap/capok',
                'value': ""
            },
            'BatteryCharge_%': {
                'command': 'cat sys/class/power_supply/bq27750-0/capacity',
                'value': ""
            },
            'BatteryVoltage_uV': {
                'command': 'cat sys/class/power_supply/bq27750-0/voltage_now',
                'value': ""
            }
        }


        self._sx5_config_dict: dict
        self._sx5_device: AdbDeviceTcp

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
        num_of_try = 60

        # Start Timer
        timer = Timer()
        timer.start()

        while (count < num_of_try):
            if timer.elapsed_time_s(2) >= 1:
                print("- Waiting for SX5 device " + "."*count, end='\r')
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

    def _read_shell(self, cmd: str):
        """"""       
        max_attempt = 10
        attempt_count = 0
        stdout = ""

        while attempt_count < max_attempt:
            try:
                stdout = self._sx5_device.shell(cmd)
            except:
                # Try to establish a new connection if no response...
                try:
                    self._sx5_device = AdbDeviceTcp(host=self._sx5_config_dict['SX5']['ip'],
                                                    port=int(self._sx5_config_dict['SX5']['port']),
                                                    default_transport_timeout_s=9.)
                except:
                    pass
                try:
                    self._sx5_device.connect(auth_timeout_s=0.2)
                except:
                    pass

                attempt_count += 1
            else:
                break

        if attempt_count >= max_attempt:
            raise exceptions.TcpTimeoutException
        pass

        return stdout

    def read_sx5_value(self, value: str):
        """ """
        if value in self._sx5_shell_values_dict.keys():
            self._sx5_shell_values_dict[value]['value'] = self._read_shell(self._sx5_shell_values_dict[value]['command'])
        return

    def update_all_sx5_values(self):
        """ """
        # Variables
        concat_cmd = ""
        stdout = ""

        for key in self._sx5_shell_values_dict:
            concat_cmd += self._sx5_shell_values_dict[key]['command'] + " && "
        
        # Delete last "&&"
        concat_cmd = concat_cmd[: -3]

        # Read output from adb shell
        stdout = self._read_shell(concat_cmd)

        keys_list=list(self._sx5_shell_values_dict.keys())
        stdout_splitted = stdout.split("\n")[: -1]
        
        # Add each element of stdout to the proper value of shel dictionary
        for index, item in enumerate(stdout_splitted):
            self._sx5_shell_values_dict[keys_list[index]]["value"] = stdout_splitted[index]            
        return
        

    @property
    def supercap_voltage_mV(self):
        return int(self._sx5_shell_values_dict['SupercapVoltage_mV']['value'].strip(" mV\n"))

    @property
    def capok_flag(self):
        return bool(int(self._sx5_shell_values_dict['CapokFlag']['value'].strip("\n")))

    @property
    def battery_charge_pct(self):
        return int(self._sx5_shell_values_dict['BatteryCharge_%']['value'].strip("\n"))

    @property
    def battery_voltage_uV(self):
        return int(self._sx5_shell_values_dict['BatteryVoltage_uV']['value'].strip(" mV\n"))

    @property
    def battery_voltage_mV(self):
        return self.battery_voltage_uV / 1000


if __name__ == "__main__":
    import time
    from Sources.Libraries.Timer import Timer
    test = SX5_Manager()
    test.init()

    test.update_all_sx5_values()

    print (test.supercap_voltage_mV)
    print (test.capok_flag)
    print (test.battery_charge_pct)
    print (test.battery_voltage_mV)











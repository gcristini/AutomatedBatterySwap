from CustomSerial import CustomSerial
from Timer import Timer
from ParseXml import XmlDictConfig
from xml.etree import ElementTree

class RelayStatusEnum:
    RS_ON = "on"
    RS_OFF = "off"


class AutomatedBatterySwap(object):

    # ****************************** #
    # ****** Private Methods ******* #
    # ****************************** #
    def __init__(self):
        """"""
        self._config_dict = None
        self._serial_relay = None
        self._global_timer = None

        self._relay_status = RelayStatusEnum.RS_OFF
        pass

    def _parse_config_file(self):
        """"""
        self._config_dict = XmlDictConfig(ElementTree.parse('Config.xml').getroot())

        pass

    def _serial_relay_init(self):
        """"""
        self._serial_relay = CustomSerial(port=self._config_dict["Serial"]["com"],
                                          baudrate=self._config_dict["Serial"]["baudarate"])
        self._serial_relay.serial_init()
        pass

    def _global_timer_init(self):
        """"""
        self._global_timer = Timer()
        pass

    def relay_on(self):
        """"""
        print("Relay On")
        self._serial_relay.serial_write("on\r")
        self._relay_status = RelayStatusEnum.RS_ON

        pass

    def relay_off(self):
        """"""
        print("Relay Off")
        self._serial_relay.serial_write("off\r")
        self._relay_status = RelayStatusEnum.RS_OFF
        pass

    def _toggle_relay(self):
        if self._relay_status == RelayStatusEnum.RS_ON:
            self.relay_off()
        elif self._relay_status == RelayStatusEnum.RS_OFF:
            self.relay_on()

    # ****************************** #
    # ******* Public Methods ******* #
    # ****************************** #

    def init(self):

        # Parse config file
        self._parse_config_file()

        # Initialize serial relay
        self._serial_relay_init()

        # Initialize global timer
        self._global_timer_init()
        pass

    def run(self):
        #self.relay_on()
        self._global_timer.start()

        while True:
            if (self._global_timer.elapsed_time_s >= int(self._config_dict["Loop"]["time_s"])):
                self._toggle_relay()
                self._global_timer.reset()

        pass


if __name__ == '__main__':
    print("--- Create Class ---")
    test = AutomatedBatterySwap()

    print("--- Init ---")
    test.init()

    print("--- Run ---")
    test.run()


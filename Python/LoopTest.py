from SerialRelay import SerialRelay
from Timer import *
from ParseXml import XmlDictConfig
from xml.etree import ElementTree
from Enums import Enums as en
from SX5_Manager import SX5_Manager
import colorama as cm
import sys
import adb_shell.exceptions


class LoopTest(object):
    """"""
    def __init__(self):
        """"""
        # Global Variables
        self._lt_config_dict: dict
        self._serial_relay: SerialRelay
        self._global_timer: Timer
        self._supercap_sample_timer: Timer
        self._sx5: SX5_Manager

        self._lt_state_machine_fun_dict = {
            en.LoopTestStateEnum.LT_STATE_INIT: self._init_state_manager,
            en.LoopTestStateEnum.LT_STATE_ON: self._on_state_manager,
            en.LoopTestStateEnum.LT_STATE_OFF: self._off_state_manager,
            en.LoopTestStateEnum.LT_STATE_SUPERCAP_MONITOR: self._supercap_monitor_state_manager,
            en.LoopTestStateEnum.LT_STATE_STOP: self._stop_state_manager,
        }

        self._lt_state: en.LoopTestStateEnum
        self._last_lt_state: en.LoopTestStateEnum
        self._lt_cmd: en.LoopTestCommandsEnum
        self._last_lt_cmd: en.LoopTestCommandsEnum
        self._exit_condition: bool

        self._relays_toggle_status: en.RelayStatusEnum
        self._current_loop: int
        self._supercap_voltage_mV: int

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def _parse_config_file(self):
        """"""
        self._lt_config_dict = XmlDictConfig(ElementTree.parse('LT_Config.xml').getroot())

        pass

    def _serial_relay_init(self):
        """ Initialize all relay to ON state """
        self._serial_relay = SerialRelay(port=self._lt_config_dict["Serial"]["com"],
                                         baudrate=self._lt_config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        self._relays_toggle_status = en.RelayStatusEnum.RS_ON
        pass

    def _timers_init(self):
        """"""
        # Initialize Global Timer
        self._global_timer = Timer()

        # Initialize Supercap Sample Timer
        self._supercap_sample_timer = Timer()

        pass

    def _sx5_init(self):
        """"""
        # Initialize SX5 device
        self._sx5 = SX5_Manager()
        self._sx5.init()
        pass

    # ------------------ STATE MACHINE ------------------ #
    def _store_last_state(self):
        """"""
        self._last_lt_state = self._lt_state
        pass

    def _init_state_manager(self):
        """ Init State Manager """

        # Start from iteration 0
        self._current_loop = 0

        # Start timers
        self._global_timer.start()
        self._supercap_sample_timer.start()

        # Store last state
        self._store_last_state()

        # Go to supercap monitor state
        self._lt_state = en.LoopTestStateEnum.LT_STATE_SUPERCAP_MONITOR
        pass

    def _on_state_manager(self):
        """ Relay On State Manager """
        if not self._exit_condition:

            # Relays on
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)
            self._relays_toggle_status = en.RelayStatusEnum.RS_ON

            # Increase current loop
            self._current_loop += 1

            # Store last state
            self._last_lt_state = self._lt_state
            # Go to on state manager
            self._lt_state = en.LoopTestStateEnum.LT_STATE_SUPERCAP_MONITOR

        else:
            # Save last state
            self._last_lt_state = self._lt_state
            # Go to stop state
            self.lt_state = en.LoopTestStateEnum.LT_STATE_STOP

        return

    def _off_state_manager(self):
        """ Relay Off State Manager """
        if not self._exit_condition:
            print("\n--- Iteration n°{iter} ---".format(iter=self._current_loop))

            # Put all relay at off state
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_OFF)

            # Relay are in off state
            self._relays_toggle_status = en.RelayStatusEnum.RS_OFF

            # Save last state
            self._last_lt_state = self._lt_state

            # Go to supercap monitor state
            self._lt_state = en.LoopTestStateEnum.LT_STATE_SUPERCAP_MONITOR
        else:
            # Save last state
            self._last_lt_state = self._lt_state

            # Go to stop state
            self._lt_state = en.LoopTestStateEnum.LT_STATE_STOP

        return

    def _supercap_monitor_state_manager(self):
        """"""
        if not self._exit_condition:
            if self._supercap_sample_timer.elapsed_time_ms() >= int(self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                # Read supercap voltage
                try:
                    self._sx5.read_supercap_voltage_mV()
                    self._supercap_voltage_mV = self._sx5.supercap_voltage_mV

                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print("Supercap voltage: {voltage}mV".format(voltage=self._supercap_voltage_mV), end="\r")

                    # Check if one of threshold as been reached
                    if (self._supercap_voltage_mV >= int(self._lt_config_dict["SX5"]["supercap_th_high_mv"]) and
                            self._relays_toggle_status == en.RelayStatusEnum.RS_ON):

                        # Save last state
                        self._last_lt_state = self._lt_state

                        # Go to relay off state
                        self._lt_state = en.LoopTestStateEnum.LT_STATE_OFF

                    elif (self._supercap_voltage_mV <= int(self._lt_config_dict["SX5"]["supercap_th_low_mv"]) and
                            self._relays_toggle_status == en.RelayStatusEnum.RS_OFF):

                        # Save last state
                        self._last_lt_state = self._lt_state

                        # Go to relay on state
                        self._lt_state = en.LoopTestStateEnum.LT_STATE_ON

                    else:
                        pass
                    # Reset timer
                    self._supercap_sample_timer.reset()

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    self._lt_cmd = en.LoopTestCommandsEnum.LT_CMD_STOP

        else:
            # Save last state
            self._last_lt_state = self._lt_state

            # Go to stop state
            self._lt_state = en.LoopTestStateEnum.LT_STATE_STOP

        return

    def _stop_state_manager(self):
        """"""
        # Put all relays on
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        # Stop timers
        self._global_timer.stop()
        self._supercap_sample_timer.stop()

        # Close serial relay port
        self._serial_relay.close()

        # Store last state
        self._last_lt_state = self._lt_state
        pass

    def _lt_state_machine_manager(self):
        """"""
        # Get function from dictionary
        fun = self._lt_state_machine_fun_dict.get(self._lt_state)

        # Execute function
        fun()
        pass

    # ---------------------------------------------------------------- #
    # ------------------------ Public Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def init(self):
        """ Initialize Loop Test """
        # Initialize Colorama library
        cm.init(autoreset=True)

        # Parse config file
        self._parse_config_file()

        # Initialize Timers
        self._timers_init()

        # Initialize Relays On
        self._serial_relay_init()

        # Initialize SX5
        self._sx5_init()

        # Initialize state variables
        self._lt_state = en.LoopTestStateEnum.LT_STATE_INIT
        self._exit_condition = False

        pass

    def run(self):
        """ """
        # Force Command to start
        self._lt_cmd = en.LoopTestCommandsEnum.LT_CMD_START

        print(cm.Fore.CYAN + cm.Style.DIM + "\n----------------------------------------")
        print(cm.Fore.CYAN + cm.Style.DIM + "*** Run Loop Test ***")

        while not (self._lt_state == en.LoopTestStateEnum.LT_STATE_STOP and
                   self._last_lt_state == en.LoopTestStateEnum.LT_STATE_STOP):
            # Run state machine at the current state
            self._lt_state_machine_manager()

            # Evaluate exit condition
            self._exit_condition = (self._global_timer.elapsed_time_min() >= float(self._lt_config_dict["Loop"]["time_test_min"]) or
                                    (self._current_loop > int(self._lt_config_dict["Loop"]["n_loop"]) - 1) or
                                     (self._lt_cmd == en.LoopTestCommandsEnum.LT_CMD_STOP))

        # Reset state machine variables
        self._lt_state = en.LoopTestStateEnum.LT_STATE_INIT
        self._last_lt_state = en.LoopTestStateEnum.LT_STATE_INIT

        print("\n--- Finished ----")
        print("-Tot. loops: {loop}".format(loop=self._current_loop))
        print("-Elapsed Time: {time} min".format(time=self._global_timer.elapsed_time_min()))

        print(cm.Fore.CYAN + cm.Style.DIM + "\n*** Exit Loop Test ***")
        print(cm.Fore.CYAN + cm.Style.DIM + "----------------------------------------\n")
        pass

    def stop(self):
        """ """
        self._lt_cmd = en.LoopTestCommandsEnum.LT_CMD_STOP
        pass


if __name__ == '__main__':
    test=LoopTest()

    test.init()
    test.run()
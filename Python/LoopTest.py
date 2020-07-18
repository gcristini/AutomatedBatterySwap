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
        self._timers_dict = {
            'GlobalTimer': Timer,
            'SupercapSampleTimer': Timer,
            'ChargeTimer': Timer,
            'DischargeTimer': Timer
        }
        self._sx5: SX5_Manager

        # State Machine
        self._lt_state_machine_fun_dict = {
            en.LoopTestStateEnum.LT_STATE_INIT: self._init_state_manager,
            en.LoopTestStateEnum.LT_STATE_ON: self._on_state_manager,
            en.LoopTestStateEnum.LT_STATE_OFF: self._off_state_manager,
            en.LoopTestStateEnum.LT_STATE_STOP: self._stop_state_manager,
        }

        self._lt_state = None  # en.LoopTestStateEnum
        self._last_lt_state = None  # en.LoopTestStateEnum
        self._lt_cmd = None  # en.LoopTestCommandsEnum

        self._exit_condition = None  # bool
        self._relays_toggle_status = None  # en.RelayStatusEnum
        self._current_loop: int
        self._supercap_voltage_mV = {
            'Start': int,
            'Stop': int,
            'Current': int
        }

        self._lt_exit_cause_dict = {
            'Elapsed Time': False,
            'Max Num of Loops Reached': False,
            'Stop Command': False,
            'Device Disconnected': False
        }

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def _parse_config_file(self):
        """"""
        self._lt_config_dict = XmlDictConfig(ElementTree.parse('LT_Config.xml').getroot())

        pass

    def _serial_relay_init(self):
        """ Initialize all relays to ON state """
        self._serial_relay = SerialRelay(port=self._lt_config_dict["Serial"]["com"],
                                         baudrate=self._lt_config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        self._relays_toggle_status = en.RelayStatusEnum.RS_ON
        pass

    def _timers_init(self):
        """"""
        # Initialize all Timers
        for timer in list(self._timers_dict):
            self._timers_dict[timer] = Timer()
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
        # Store last state
        self._last_lt_state = self._lt_state
        pass

    def _go_to_next_state(self, state):
        """"""
        # Store last state
        self._store_last_state()

        # Go to next state
        self._lt_state = state
        pass

    def _update_exit_condition(self, cause, flag):
        """"""
        # Update the selected cause
        self._lt_exit_cause_dict[cause] = flag

        # Update the exit condition flag doing the or of all flags
        self._exit_condition = sum(self._lt_exit_cause_dict.values())

        pass

    def _init_state_manager(self):
        """ Init State Manager """
        if (self._lt_state == en.LoopTestStateEnum.LT_STATE_INIT and
                self._last_lt_state != en.LoopTestStateEnum.LT_STATE_INIT):

            # Start from iteration 0
            self._current_loop = 0

            # Start global and supercap timers
            self._timers_dict['GlobalTimer'].start()
            self._timers_dict['SupercapSampleTimer'].start()

            # Store the last state
            self._store_last_state()
        else:
            if self._timers_dict['SupercapSampleTimer'].elapsed_time_ms() >= int(
                    self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                try:
                    # Read supercap voltage
                    self._sx5.read_supercap_voltage_mV()
                    self._supercap_voltage_mV['Current'] = self._sx5.supercap_voltage_mV

                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print("Waiting for supercap fully charged before start loop.... Voltage: {voltage}mV".format(
                        voltage=self._supercap_voltage_mV['Current']),
                        end="\r")

                    if self._supercap_voltage_mV['Current'] >= int(self._lt_config_dict["SX5"]["supercap_th_high_mv"]):
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print("Supercap fully charged ({voltage}mV): Start Loop!\n".format(voltage=self._supercap_voltage_mV['Current']))

                        # Go to relay off state
                        self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_OFF)

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    self._update_exit_condition('Device Disconnected', True)
        return

    def _on_state_manager(self):
        """ Relay On State Manager """
        if not self._exit_condition:
            # If sample time is elapsed
            if self._timers_dict['SupercapSampleTimer'].elapsed_time_ms() >= int(self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                try:
                    # Read supercap voltage
                    self._sx5.read_supercap_voltage_mV()
                    self._supercap_voltage_mV['Current'] = self._sx5.supercap_voltage_mV

                    # On the transition to this state...
                    if (self._lt_state == en.LoopTestStateEnum.LT_STATE_ON and
                            self._last_lt_state != en.LoopTestStateEnum.LT_STATE_ON):

                        # Relays on
                        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)
                        self._relays_toggle_status = en.RelayStatusEnum.RS_ON

                        # Start discharge timer
                        self._timers_dict['ChargeTimer'].start()

                        # Store the current supercap voltage as start voltage
                        self._supercap_voltage_mV['Start'] = self._supercap_voltage_mV['Current']

                        # Store last state
                        self._store_last_state()
                    else:
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print("Charging.... supercap voltage: {voltage}mV".format(voltage=self._supercap_voltage_mV['Current']),
                                                                                  end="\r")

                        # Check if the high threshold has been reached
                        if self._supercap_voltage_mV['Current'] >= int(self._lt_config_dict["SX5"]["supercap_th_high_mv"]):

                            # Increase current loop
                            self._current_loop += 1

                            # Stop charge timer
                            self._timers_dict['ChargeTimer'].stop()

                            # Store the current supercap voltage as stop voltage
                            self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']

                            # Print Discharge time
                            sys.stdout.write("\033[K")  # Clear to the end of line
                            print("- Charge:")
                            print("\t• time: {time}s".format(time=self._timers_dict['ChargeTimer'].elapsed_time_s(digits=2)))
                            print("\t• start voltage: {start}mV".format(start=self._supercap_voltage_mV['Start']))
                            print("\t• stop voltage: {stop}mV".format(stop=self._supercap_voltage_mV['Stop']))

                            # Go to relay off state
                            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_OFF)

                    # Reset timer
                    self._timers_dict['SupercapSampleTimer'].reset()

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    self._update_exit_condition('Device Disconnected', True)

        else:
            # Go to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

        return

    def _off_state_manager(self):
        """ Relay Off State Manager """
        if not self._exit_condition:

            # If Sample time is elapsed
            if self._timers_dict['SupercapSampleTimer'].elapsed_time_ms() >= int(self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                # Read supercap voltage
                try:
                    self._sx5.read_supercap_voltage_mV()
                    self._supercap_voltage_mV['Current'] = self._sx5.supercap_voltage_mV

                    # On the transition to this state...
                    if (self._lt_state == en.LoopTestStateEnum.LT_STATE_OFF and
                            self._last_lt_state != en.LoopTestStateEnum.LT_STATE_OFF):

                        print("\n--- Iteration n°{iter} ---".format(iter=self._current_loop))

                        # Put all relay at off state
                        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_OFF)

                        # Relay are in off state
                        self._relays_toggle_status = en.RelayStatusEnum.RS_OFF

                        # Start discharge timer
                        self._timers_dict['DischargeTimer'].start()

                        # Store the current supercap voltage as start voltage
                        self._supercap_voltage_mV['Start'] = self._supercap_voltage_mV['Current']

                        # Store last state
                        self._store_last_state()
                    else:
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print("Discharging... supercap voltage: {voltage}mV".format(voltage=self._supercap_voltage_mV['Current']),
                                                                                    end="\r")

                        # Check if the low threshold has been reached
                        if self._supercap_voltage_mV['Current'] <= int(self._lt_config_dict["SX5"]["supercap_th_low_mv"]):

                            # Stop discharge timer
                            self._timers_dict['DischargeTimer'].stop()

                            # Store the current supercap voltage as stop voltage
                            self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']

                            # Print Discharge time
                            sys.stdout.write("\033[K")  # Clear to the end of line
                            print("- Discharge:")
                            print("\t• time: {time}s".format(time=self._timers_dict['DischargeTimer'].elapsed_time_s(digits=2)))
                            print("\t• start voltage: {start}mV".format(start=self._supercap_voltage_mV['Start']))
                            print("\t• stop voltage: {stop}mV".format(stop=self._supercap_voltage_mV['Stop']))

                            # Go to relay on state
                            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_ON)

                    # Reset supercap sample timer
                    self._timers_dict['SupercapSampleTimer'].reset()

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    self._update_exit_condition('Device Disconnected', True)
        else:
            # Go to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

        return

    def _stop_state_manager(self):
        """"""
        # Put all relays on
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        # Stop all timers
        for timer in list(self._timers_dict):
            try:
                self._timers_dict[timer].stop()
            except TimerError:
                pass

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

            # Update exit condition
            self._update_exit_condition('Elapsed Time', (self._timers_dict['GlobalTimer'].elapsed_time_min() >= float(self._lt_config_dict["Loop"]["time_test_min"])))
            self._update_exit_condition('Max Num of Loops Reached', (self._current_loop > int(self._lt_config_dict["Loop"]["n_loop"]) - 1))
            self._update_exit_condition('Stop Command', (self._lt_cmd == en.LoopTestCommandsEnum.LT_CMD_STOP))

        # Reset state machine variables
        self._lt_state = en.LoopTestStateEnum.LT_STATE_INIT
        self._last_lt_state = en.LoopTestStateEnum.LT_STATE_INIT

        print("\n--- Finished ----")
        print("-Tot. loops: {loop}".format(loop=self._current_loop))
        print("-Elapsed Time: {time} min".format(time=self._timers_dict['GlobalTimer'].elapsed_time_min(digits=2)))
        print ("\n-Test finished for: {cause}".format(cause=[key for key, value in self._lt_exit_cause_dict.items() if value]))
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
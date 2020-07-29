from SerialRelay import SerialRelay
from Timer import *
from ParseXml import XmlDictConfig
from xml.etree import ElementTree
from Enums import Enums as en
from SX5_Manager import SX5_Manager
import colorama as cm
import sys
import adb_shell.exceptions
import csv
import time
import random
from TektronixManager import TektronixManager


# noinspection PyTypeChecker
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
        self._osc: TektronixManager

        # State Machine
        self._lt_state_machine_fun_dict = {
            en.LoopTestStateEnum.LT_STATE_INIT: self._init_state_manager,
            en.LoopTestStateEnum.LT_STATE_ON: self._on_state_manager,
            en.LoopTestStateEnum.LT_STATE_UPDATE_CSV: self._update_csv_state_manager,
            en.LoopTestStateEnum.LT_STATE_OFF: self._off_state_manager,
            en.LoopTestStateEnum.LT_STATE_STOP: self._stop_state_manager,
        }

        self._lt_state = None  # en.LoopTestStateEnum
        self._last_lt_state = None  # en.LoopTestStateEnum
        self._lt_cmd = None  # en.LoopTestCommandsEnum
        self._loop_result = None
        self._exit_condition = None  # bool

        self._current_loop: int
        self._supercap_voltage_mV = {
            'Start': int,
            'Stop': int,
            'Current': int
        }

        self._capok_flag = bool

        self._lt_exit_cause_dict = {
            'Elapsed Time': False,
            'Max Num of Loops Reached': False,
            'Stop Command': False,
            'Device Disconnected': False,
            'Charge Timeout': False
        }

        self._csv_log_dict = {
            "filename": None,
            "file": None,
            "csv_writer": None,
            "log_data": {
                'Iteration': int,
                'Discharge Time [s]': int,
                'Charge Time [s]': int,
                'Total Time [s]': int,
                'Start Voltage (Discharge) [mV]': int,
                'Stop Voltage (Discharge) [mV]': int,
                'Pinout sequence (Discharge)': str,
                'Start Voltage (Charge) [mV]': int,
                'Stop Voltage (Charge) [mV]': int,
                'Pinout sequence (Charge)': str,
                'Result': None
            }
        }

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #

    def _parse_config_file(self):
        """"""
        self._lt_config_dict = XmlDictConfig(ElementTree.parse('Config.xml').getroot())['Global']
        self._lt_config_dict.update(XmlDictConfig(ElementTree.parse('Config.xml').getroot())['LoopTest'])

        pass

    def _oscillocope_init(self):
        """"""
        self._osc = TektronixManager(resource_name=self._lt_config_dict['Oscilloscope']['resource_name'])
        self._osc.init()
        pass

    def _serial_relay_init(self):
        """ Initialize all relays to ON state """
        self._serial_relay = SerialRelay(port=self._lt_config_dict["Serial"]["com"],
                                         baudrate=self._lt_config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        self._serial_relay.drive_relay(cmd=en.DebugRelayCommandsEnum.RC_ALL_ON)
        #self._serial_relay.drive_relay(cmd=self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_ON))

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

    @staticmethod
    def _generate_random_relay_command(relay_status=None):
        relays_list = en.RelaysEnum.values()
        random.shuffle(relays_list)

        relay_cmd = ""

        # Append all relay in the list
        for relay in relays_list:
            relay_cmd += relay + "_"

        # Delete last character
        relay_cmd = relay_cmd[:-1]

        # Append the relay status
        relay_cmd += " " + relay_status
        print(relay_cmd)

        return relay_cmd

    def _init_state_manager(self):
        """ Init State Manager """
        if (self._lt_state == en.LoopTestStateEnum.LT_STATE_INIT and
                self._last_lt_state != en.LoopTestStateEnum.LT_STATE_INIT):

            # Start from iteration 1
            self._current_loop = 1

            # Start global and supercap timers
            self._timers_dict['GlobalTimer'].start()
            self._timers_dict['SupercapSampleTimer'].start()

            # Init Log file and write headers of dictionary
            self._csv_log_dict['filename'] = "{name}_{date}{ext}".format(name=self._lt_config_dict['Log']['csv_filename'].strip(".csv"),
                                                                         date=time.strftime("%d%m_%H%M"),
                                                                         ext=".csv")

            self._csv_log_dict['file'] = open(file=self._csv_log_dict['filename'], mode='w',  newline='')
            #self._csv_file = open(self._lt_config_dict['Log']['csv_filename'], 'w')
            self._csv_log_dict['csv_writer'] = csv.writer(self._csv_log_dict['file'])
            self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict["log_data"].keys())
            self._csv_log_dict['file'].close()

            # Store the last state
            self._store_last_state()
        else:
            if self._timers_dict['SupercapSampleTimer'].elapsed_time_ms() >= int(
                    self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                try:
                    # Read supercap voltage and cap ok flag
                    self._sx5.read_supercap_voltage_mV()
                    self._sx5.read_capok_flag()

                    self._supercap_voltage_mV['Current'] = self._sx5.supercap_voltage_mV
                    self._capok_flag = self._sx5.capok_flag

                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "Waiting for supercap fully charged before start loop.... Voltage: {voltage}mV".format(
                        voltage=self._supercap_voltage_mV['Current']),
                        end="\r")

                    if ((self._capok_flag is True) and \
                            (self._timers_dict['GlobalTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):
                    #
                    # if ((self._supercap_voltage_mV['Current'] >= int(self._lt_config_dict["SX5"]["supercap_th_high_mv"])) and
                    #     (self._timers_dict['GlobalTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):

                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print(cm.Fore.CYAN + cm.Style.DIM + "Supercap fully charged @{voltage}mV: Start Loop!".format(voltage=self._supercap_voltage_mV['Current']))

                        # Go to relay off state
                        self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_OFF)
                    elif self._timers_dict['GlobalTimer'].elapsed_time_min() > float(self._lt_config_dict['SX5']['charge_timeout_min']):
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print(cm.Fore.CYAN + cm.Style.DIM + "Charge timeout reached")

                        self._update_exit_condition('Charge Timeout', True)
                        self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    # Device disconnected, test failed
                    self._loop_result = "Failed"
                    self._update_exit_condition('Device Disconnected', True)
                    # Go to update csv state and then to stop
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)
        return

    def _on_state_manager(self):
        """ Relay On State Manager """
        if not self._exit_condition:
            # If sample time is elapsed
            if self._timers_dict['SupercapSampleTimer'].elapsed_time_ms() >= int(self._lt_config_dict["SX5"]["supercap_sample_time_ms"]):
                try:
                    # Read supercap voltage
                    self._sx5.read_supercap_voltage_mV()
                    self._sx5.read_capok_flag()

                    self._supercap_voltage_mV['Current'] = self._sx5.supercap_voltage_mV
                    self._capok_flag = self._sx5.capok_flag

                    # On the transition to this state...
                    if (self._lt_state == en.LoopTestStateEnum.LT_STATE_ON and
                            self._last_lt_state != en.LoopTestStateEnum.LT_STATE_ON):

                        # Relays on
                        if bool(self._lt_config_dict['SX5']['random_relay']) is True:
                            relay_cmd = self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_ON)
                        else:
                            relay_cmd = "{relay} {status}".format(relay="packp_scl_sda_detect_ntc",
                                                                  status=en.RelayStatusEnum.RS_ON)
                        # Store the relays command and drive relays
                        self._csv_log_dict['log_data']['Pinout sequence (Charge)'] = relay_cmd
                        self._serial_relay.drive_relay(cmd=relay_cmd)

                        # Start discharge timer
                        self._timers_dict['ChargeTimer'].start()

                        # Store the current supercap voltage as start voltage
                        self._supercap_voltage_mV['Start'] = self._supercap_voltage_mV['Current']

                        # Store image on oscilloscope
                        self._osc.image_name = "Loop_{num}".format(num=self._current_loop)
                        self._osc.save_screen()

                        # Store last state
                        self._store_last_state()
                    else:
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print(cm.Fore.CYAN + cm.Style.DIM + "Charging.... supercap voltage: {voltage}mV".format(voltage=self._supercap_voltage_mV['Current']),
                                                                                                                end="\r")
                        # Check if the high threshold has been reached
                        if ((self._sx5.capok_flag is True) and
                                (self._timers_dict['ChargeTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):

                        # if ((self._supercap_voltage_mV['Current'] >= int(self._lt_config_dict["SX5"]["supercap_th_high_mv"])) and
                        #         (self._timers_dict['GlobalTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):
                            # Increase current loop
                            self._current_loop += 1

                            # Stop charge timer
                            self._timers_dict['ChargeTimer'].stop()

                            # Store the current supercap voltage as stop voltage
                            self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']

                            # Print Discharge time
                            sys.stdout.write("\033[K")  # Clear to the end of line
                            print(cm.Fore.CYAN + cm.Style.DIM + "- Charge:")
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• time: {time}s".format(time=self._timers_dict['ChargeTimer'].elapsed_time_s(digits=0)))
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• start voltage: {start}mV".format(start=self._supercap_voltage_mV['Start']))
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• stop voltage: {stop}mV".format(stop=self._supercap_voltage_mV['Stop']))

                            # A discharge-charge loop has been completed: Pass!
                            self._loop_result = "Passed"

                            # Go to update csv state
                            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

                        elif self._timers_dict['ChargeTimer'].elapsed_time_min() > float(self._lt_config_dict['SX5']['charge_timeout_min']):
                            sys.stdout.write("\033[K")  # Clear to the end of line
                            print(cm.Fore.CYAN + cm.Style.DIM + "Charge timeout reached")
                            self._update_exit_condition('Charge Timeout', True)
                            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

                    # Reset timer
                    self._timers_dict['SupercapSampleTimer'].reset()

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    # Device disconnected, test failed
                    self._loop_result = "Failed"
                    self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']
                    print(self._supercap_voltage_mV['Current'])
                    self._update_exit_condition('Device Disconnected', True)

        else:
            # Go to update csv state and then to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

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

                        print(cm.Fore.CYAN + cm.Style.DIM + "\n--- Loop {iter}/{max_iter} ---".format(iter=self._current_loop,
                                                                                                      max_iter=self._lt_config_dict["Loop"]["n_loop"]))

                        # Put all relay at off state
                        if bool(self._lt_config_dict['SX5']['random_relay']) is True:
                            relay_cmd = self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_OFF)
                        else:
                            relay_cmd = "{relay} {status}".format(relay="packp_scl_sda_detect_ntc",
                                                                  status=en.RelayStatusEnum.RC_ALL_OFF)
                        # Store relays command and drive relays
                        self._csv_log_dict['log_data']['Pinout sequence (Discharge)'] = relay_cmd
                        self._serial_relay.drive_relay(cmd=relay_cmd)

                        # Start discharge timer
                        self._timers_dict['DischargeTimer'].start()

                        # Store the current supercap voltage as start voltage
                        self._supercap_voltage_mV['Start'] = self._supercap_voltage_mV['Current']

                        # Store last state
                        self._store_last_state()
                    else:
                        sys.stdout.write("\033[K")  # Clear to the end of line
                        print(cm.Fore.CYAN + cm.Style.DIM + "Discharging... supercap voltage: {voltage}mV".format(voltage=self._supercap_voltage_mV['Current']),
                                                                                                                  end="\r")

                        # Check if the low threshold has been reached
                        if self._supercap_voltage_mV['Current'] <= int(self._lt_config_dict["SX5"]["supercap_th_low_mv"]):

                            # Stop discharge timer
                            self._timers_dict['DischargeTimer'].stop()

                            # Store the current supercap voltage as stop voltage
                            self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']

                            # Print Discharge time
                            sys.stdout.write("\033[K")  # Clear to the end of line
                            print(cm.Fore.CYAN + cm.Style.DIM + "- Discharge:")
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• time: {time}s".format(time=self._timers_dict['DischargeTimer'].elapsed_time_s(digits=0)))
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• start voltage: {start}mV".format(start=self._supercap_voltage_mV['Start']))
                            print(cm.Fore.CYAN + cm.Style.DIM + "\t• stop voltage: {stop}mV".format(stop=self._supercap_voltage_mV['Stop']))

                            # Go to update csv state
                            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

                    # Reset supercap sample timer
                    self._timers_dict['SupercapSampleTimer'].reset()

                # Device not respond
                except adb_shell.exceptions.TcpTimeoutException:
                    # Device disconnected, test failed
                    self._loop_result = "Failed"
                    self._supercap_voltage_mV['Stop'] = self._supercap_voltage_mV['Current']
                    self._update_exit_condition('Device Disconnected', True)
        else:
            # Go to update csv state and then to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

        return

    def _update_csv_state_manager(self):
        """ Update tge CSV log file """
        self._csv_log_dict['file'] = open(file=self._csv_log_dict['filename'], mode='a',  newline='')
        self._csv_log_dict['csv_writer'] = csv.writer(self._csv_log_dict['file'])

        # Update Result field
        self._csv_log_dict['log_data']['Result'] = self._loop_result

        if (self._lt_state == en.LoopTestStateEnum.LT_STATE_UPDATE_CSV and
                self._last_lt_state == en.LoopTestStateEnum.LT_STATE_OFF):
            # Populate csv dict
            self._csv_log_dict['log_data']['Iteration'] = self._current_loop
            self._csv_log_dict['log_data']['Discharge Time [s]'] = self._timers_dict['DischargeTimer'].elapsed_time_s(digits=2)
            self._csv_log_dict['log_data']['Start Voltage (Discharge) [mV]'] = self._supercap_voltage_mV['Start']
            self._csv_log_dict['log_data']['Stop Voltage (Discharge) [mV]'] = self._supercap_voltage_mV['Stop']

            if self._loop_result == "Failed":
                # Update csv log file
                self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict['log_data'].values())

            # Go to relay on state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_ON)

        elif (self._lt_state == en.LoopTestStateEnum.LT_STATE_UPDATE_CSV and
                self._last_lt_state == en.LoopTestStateEnum.LT_STATE_ON):
            # Populate csv dict
            self._csv_log_dict['log_data']['Charge Time [s]'] = self._timers_dict['ChargeTimer'].elapsed_time_s(digits=2)
            self._csv_log_dict['log_data']['Total Time [s]'] = self._csv_log_dict['log_data']['Charge Time [s]'] + \
                                                               self._csv_log_dict['log_data']['Discharge Time [s]']
            self._csv_log_dict['log_data']['Start Voltage (Charge) [mV]'] = self._supercap_voltage_mV['Start']
            self._csv_log_dict['log_data']['Stop Voltage (Charge) [mV]'] = self._supercap_voltage_mV['Stop']

            # Update csv log file
            self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict['log_data'].values())

            # Clear csv dictionary
            for item in self._csv_log_dict['log_data']:
                self._csv_log_dict['log_data'][item] = None

            # Go to relay off state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_OFF)

        elif (self._lt_state == en.LoopTestStateEnum.LT_STATE_UPDATE_CSV and
              self._last_lt_state == en.LoopTestStateEnum.LT_STATE_INIT):

            # Update csv log file
            self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict['log_data'].values())
        else:
            pass

        if self._exit_condition:
            # Go to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

        # Close file
        self._csv_log_dict['file'].close()

        pass

    def _stop_state_manager(self):
        """"""
        # Put all relays on
        self._serial_relay.drive_relay(cmd=en.DebugRelayCommandsEnum.RC_ALL_ON)
        #self._serial_relay.drive_relay(cmd=self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_ON))

        # Stop all timers
        for timer in list(self._timers_dict):
            try:
                self._timers_dict[timer].stop()
            except TimerError:
                pass

        # Close serial relay port
        self._serial_relay.close()

        # Close csv file
        self._csv_log_dict['file'].close()

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

        # Initialize oscilloscope
        self._oscillocope_init()

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
            self._update_exit_condition('Max Num of Loops Reached', (self._current_loop > int(self._lt_config_dict["Loop"]["n_loop"])))
            self._update_exit_condition('Stop Command', (self._lt_cmd == en.LoopTestCommandsEnum.LT_CMD_STOP))

        # Reset state machine variables
        self._lt_state = en.LoopTestStateEnum.LT_STATE_INIT
        self._last_lt_state = en.LoopTestStateEnum.LT_STATE_INIT

        print(cm.Fore.CYAN + cm.Style.DIM + "\n--- Finished ----")
        print(cm.Fore.CYAN + cm.Style.DIM + "-Tot. loops: {loop}".format(loop=self._current_loop))
        print(cm.Fore.CYAN + cm.Style.DIM + "-Elapsed Time: {time} min".format(time=self._timers_dict['GlobalTimer'].elapsed_time_min(digits=2)))
        print(cm.Fore.CYAN + cm.Style.DIM + "\n-Test finished for: {cause}".format(cause=[key for key, value in self._lt_exit_cause_dict.items() if value]))
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
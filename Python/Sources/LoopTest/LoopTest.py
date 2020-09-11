import colorama as cm
from Libraries.SerialRelay import SerialRelay
from Libraries.Timer import *
from Libraries.ParseXml import XmlDictConfig
from xml.etree import ElementTree
from Libraries.Enums import Enums as en
from LoopTest.SX5_Manager import SX5_Manager
import colorama as cm
import sys
import adb_shell.exceptions
import csv
import time
import random
from LoopTest.TektronixManager import TektronixManager
from Libraries.CustomThread import CustomThread
import os


class LoopTest(object):
    """"""
    def __init__(self):
        """"""
        # Global Variables
        self._lt_config_dict: dict
        self._serial_relay: SerialRelay
        self._timers_dict = {
            'GlobalTimer': Timer,
            #'StateMachineTimer': Timer,
            'ChargeTimer': Timer,
            'DischargeTimer': Timer
        }
        self._sx5: SX5_Manager
        self._osc: TektronixManager

        self._thread_dict = {
            'oscilloscope': CustomThread,
            'sx5_read': CustomThread
        }

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

        self._sx5_shell_values_dict = {
            'SupercapVoltage_mV': {
                'Start': int,
                'Stop': int,
                'Current': int
            },
            'CapokFlag': bool,
            'BatteryCharge_%': {
                'Start': int,
                'Stop': int,
                'Current': int
            },
            'BatteryVoltage_mV': {
                'Start': int,
                'Stop': int,
                'Current': int
            }
        }

        self._lt_exit_cause_dict = {
            'Elapsed Time': False,
            'Max Num of Loops Reached': False,
            'Stop Command': False,
            'Device Disconnected': False,
            'Charge Timeout': False
        }

        self._csv_log_dict = {
            "filename": "",
            "file": None,
            "csv_writer": None,
            "log_data": {
                'Iteration': int,
                'Discharge Time [s]': int,
                'Charge Time [s]': int,
                'Total Time [s]': int,
                'Start Supercap Voltage (Discharge) [mV]': int,
                'Stop Supercap Voltage (Discharge) [mV]': int,
                'Start Battery Voltage (Discharge) [mV]': int,
                'Stop Battery Voltage (Discharge) [mV]': int,
                'Start Battery Charge (Discharge) [%]': int,
                'Stop Battery Charge (Discharge) [%]': int,
                'Pinout sequence (Discharge)': str,
                'Start Supercap Voltage (Charge) [mV]': int,
                'Stop Supercap Voltage (Charge) [mV]': int,
                'Start Battery Voltage (Charge) [mV]': int,
                'Stop Battery Voltage (Charge) [mV]': int,
                'Start Battery Charge (Charge) [%]': int,
                'Stop Battery Charge (Charge) [%]': int,
                'Pinout sequence (Charge)': str,
                'Result': None
            }
        }

        self._txt_log_dict = {
            "filename": "",
            "file": None            
        }

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #

    def _osc_save_image_runnable(self):
        # Wait a second and save screen
        time.sleep(1)
        self._osc.save_screen()
        pass

    def _parse_config_file(self):
        """"""
        self._lt_config_dict = XmlDictConfig(ElementTree.parse('Config.xml').getroot())['Global']
        self._lt_config_dict.update(XmlDictConfig(ElementTree.parse('Config.xml').getroot())['LoopTest'])

        pass

    def _oscilloscope_init(self):
        """"""
        self._osc = TektronixManager(resource_name=self._lt_config_dict['Oscilloscope']['resource_name'],
                                     images_dir="{name}_{date}".format(name=self._lt_config_dict['Oscilloscope']['save_folder'],
                                                                       date=time.strftime("%d%m_%H%M")))
        self._osc.init()
        pass

    def _serial_relay_init(self):
        """ Initialize all relays to ON state """
        self._serial_relay = SerialRelay(port=self._lt_config_dict["Serial"]["com"],
                                         baudrate=self._lt_config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        self._serial_relay.drive_relay(cmd=en.DebugRelayCommandsEnum.RC_ALL_ON)
        
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

        self._thread_dict['sx5_read'] = CustomThread(thread_name="ReadSX5_Val",
                                                     num_of_iter='inf',
                                                     runnable=self._update_sx5_shell_value_dict_runnable,
                                                     timing_ms=int(self._lt_config_dict["SX5"]["read_sample_time_ms"]))
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
        """ Generate the command to control relay in a casual order """
        # Take the list from enumeration and shuffle it
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

        return relay_cmd

    def _update_sx5_shell_value_dict_runnable(self):
        # Read all shell values
        try:
            self._sx5.update_all_sx5_values()
        except adb_shell.exceptions.TcpTimeoutException:
            # Device disconnected, test failed
            self._loop_result = "Failed"
            self._update_exit_condition('Device Disconnected', True)
        else:
            # nothing
            pass
        finally:
            # Update shell value dictionary
            self._sx5_shell_values_dict['SupercapVoltage_mV']['Current'] = self._sx5.supercap_voltage_mV
            self._sx5_shell_values_dict['CapokFlag'] = self._sx5.capok_flag
            self._sx5_shell_values_dict['BatteryCharge_%']['Current'] = self._sx5.battery_charge_pct
            self._sx5_shell_values_dict['BatteryVoltage_mV']['Current'] = self._sx5.battery_voltage_mV

        return

    def _init_state_manager(self):
        """ Init State Manager """
        if not self._exit_condition:
            if (self._lt_state == en.LoopTestStateEnum.LT_STATE_INIT and
                    self._last_lt_state != en.LoopTestStateEnum.LT_STATE_INIT):

                # Start from iteration 1
                self._current_loop = 1

                # Start global timer
                self._timers_dict['GlobalTimer'].start()                

                # Start read sx5 shell
                self._thread_dict['sx5_read'].start()

                # Create storage folder
                date = time.strftime("%d%m_%H%M")
                os.mkdir(date)

                # Init Log file and write headers of dictionary
                self._csv_log_dict['filename'] = "{date}\{name}{ext}".format(date=date,
                                                                             name=self._lt_config_dict['Log']['csv_filename'].strip(".csv"),                                                                             
                                                                             ext=".csv")

                self._csv_log_dict['file'] = open(file=self._csv_log_dict['filename'], mode='w',  newline='')
                self._csv_log_dict['csv_writer'] = csv.writer(self._csv_log_dict['file'])
                self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict["log_data"].keys())
                self._csv_log_dict['file'].close()

                # Init txt log and write used cfg params
                self._txt_log_dict['filename'] = "{date}\{name}{ext}".format(date=date,
                                                                            name="result",                                                                             
                                                                            ext=".log")
                self._txt_log_dict['file'] = open(file=self._txt_log_dict['filename'], mode='w')
                self._txt_log_dict['file'].write("--- Used config parameters ---\n")
                self._txt_log_dict['file'].write(f"read_sample_time_ms= {self._lt_config_dict['SX5']['read_sample_time_ms']} ms\n")
                self._txt_log_dict['file'].write(f"n_loop= {self._lt_config_dict['Loop']['n_loop']}\n")
                self._txt_log_dict['file'].write(f"supercap_th_low_mv= {self._lt_config_dict['SX5']['supercap_th_low_mv']}\n")
                self._txt_log_dict['file'].write("\n\n")
                self._txt_log_dict['file'].close()
                

                # Store the last state
                self._store_last_state()
            else:           
                sys.stdout.write("\033[K")  # Clear to the end of line
                print(cm.Fore.CYAN + cm.Style.DIM + "Waiting for supercap fully charged before start loop.... Voltage: {voltage}mV".format(
                    voltage=self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']),
                    end="\r")

                if ((self._sx5_shell_values_dict['CapokFlag'] is True) and \
                        (self._timers_dict['GlobalTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):
               
                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "Supercap fully charged @{voltage}mV: Start Loop!".format(voltage=self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']))

                    # Go to relay off state
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_OFF)
                elif self._timers_dict['GlobalTimer'].elapsed_time_min() > float(self._lt_config_dict['SX5']['charge_timeout_min']):
                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "Charge timeout reached")

                    self._update_exit_condition('Charge Timeout', True)
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)
        
        else:
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)
        
        return

    def _on_state_manager(self):
        """ Relay On State Manager """
        if not self._exit_condition:

            # On the transition to this state...
            if (self._lt_state == en.LoopTestStateEnum.LT_STATE_ON and
                    self._last_lt_state != en.LoopTestStateEnum.LT_STATE_ON):

                # Relays on
                if self._lt_config_dict['SX5']['random_relay'].upper() == "TRUE":
                    relay_cmd = self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_ON)
                else:
                    relay_cmd = "{relay} {status}".format(relay="packp_scl_sda_detect_ntc",
                                                            status=en.RelayStatusEnum.RS_ON)
                # Store the relays command and drive relays
                self._csv_log_dict['log_data']['Pinout sequence (Charge)'] = relay_cmd.strip(" on")

                self._serial_relay.drive_relay(cmd=relay_cmd)
                time.sleep(0.01)
                self._serial_relay.drive_relay(cmd=relay_cmd)
                time.sleep(0.01)
                self._serial_relay.drive_relay(cmd=relay_cmd)

                # Start discharge timer
                self._timers_dict['ChargeTimer'].start()

                # Store the current supercap voltage as start voltage
                self._sx5_shell_values_dict['SupercapVoltage_mV']['Start'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']
                self._sx5_shell_values_dict['BatteryCharge_%']['Start'] = self._sx5_shell_values_dict['BatteryCharge_%']['Current']
                self._sx5_shell_values_dict['BatteryVoltage_mV']['Start'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Current']

                # Store image on oscilloscope
                self._osc.image_name = "Loop_{num}".format(num=self._current_loop)
                self._thread_dict['oscilloscope'] = CustomThread(runnable=self._osc_save_image_runnable).start()


                # Store last state
                self._store_last_state()
            else:
                sys.stdout.write("\033[K")  # Clear to the end of line
                print(cm.Fore.CYAN + cm.Style.DIM + "Charging.... supercap voltage: {voltage}mV".format(voltage=self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']),
                                                                                                        end="\r")
                # Check if the high threshold has been reached
                if ((self._sx5.capok_flag is True) and
                        (self._timers_dict['ChargeTimer'].elapsed_time_min() <= float(self._lt_config_dict['SX5']['charge_timeout_min']))):

                    # Increase current loop
                    self._current_loop += 1

                    # Stop charge timer
                    self._timers_dict['ChargeTimer'].stop()

                    # Store the current supercap voltage as stop voltage
                    self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']
                    self._sx5_shell_values_dict['BatteryCharge_%']['Stop'] = self._sx5_shell_values_dict['BatteryCharge_%']['Current']
                    self._sx5_shell_values_dict['BatteryVoltage_mV']['Stop'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Current']

                    # Print Discharge time
                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "- Charge:")
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• time: {time}s".format(time=self._timers_dict['ChargeTimer'].elapsed_time_s(digits=0)))
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• start voltage: {start}mV".format(start=self._sx5_shell_values_dict['SupercapVoltage_mV']['Start']))
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• stop voltage: {stop}mV".format(stop=self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop']))

                    # A discharge-charge loop has been completed: Pass!
                    self._loop_result = "Passed"

                    # Go to update csv state
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

                elif self._timers_dict['ChargeTimer'].elapsed_time_min() > float(self._lt_config_dict['SX5']['charge_timeout_min']):
                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "Charge timeout reached")
                    self._update_exit_condition('Charge Timeout', True)
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_STOP)

        else:
            # Go to update csv state and then to stop state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)

        return

    def _off_state_manager(self):
        """ Relay Off State Manager """
        if not self._exit_condition:

            # On the transition to this state...
            if (self._lt_state == en.LoopTestStateEnum.LT_STATE_OFF and
                    self._last_lt_state != en.LoopTestStateEnum.LT_STATE_OFF):

                print(cm.Fore.CYAN + cm.Style.DIM + "\n--- Loop {iter}/{max_iter} (battery charge {charge}%) ---"
                        .format(iter=self._current_loop,
                                max_iter=self._lt_config_dict["Loop"]["n_loop"],
                                charge=self._sx5_shell_values_dict['BatteryCharge_%']['Current']))

                # Put all relay at off state
                if self._lt_config_dict['SX5']['random_relay'].upper() == "TRUE":
                    relay_cmd = self._generate_random_relay_command(relay_status=en.RelayStatusEnum.RS_OFF)
                else:
                    relay_cmd = "{relay} {status}".format(relay="packp_scl_sda_detect_ntc",
                                                            status=en.RelayStatusEnum.RS_OFF)
                # Store relays command and drive relays
                self._csv_log_dict['log_data']['Pinout sequence (Discharge)'] = relay_cmd.strip((" off"))
                self._serial_relay.drive_relay(cmd=relay_cmd)
                time.sleep(0.01)
                self._serial_relay.drive_relay(cmd=relay_cmd)
                time.sleep(0.01)
                self._serial_relay.drive_relay(cmd=relay_cmd)


                # Start discharge timer
                self._timers_dict['DischargeTimer'].start()

                # Store the current supercap voltage as start voltage
                self._sx5_shell_values_dict['SupercapVoltage_mV']['Start'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']
                self._sx5_shell_values_dict['BatteryCharge_%']['Start'] = self._sx5_shell_values_dict['BatteryCharge_%']['Current']
                self._sx5_shell_values_dict['BatteryVoltage_mV']['Start'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Current']

                # Store last state
                self._store_last_state()
            else:
                sys.stdout.write("\033[K")  # Clear to the end of line
                print(cm.Fore.CYAN + cm.Style.DIM + "Discharging... supercap voltage: {voltage}mV".format(voltage=self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']),
                                                                                                            end="\r")

                # Check if the low threshold has been reached
                if self._sx5_shell_values_dict['SupercapVoltage_mV']['Current'] <= int(self._lt_config_dict["SX5"]["supercap_th_low_mv"]):

                    # Stop discharge timer
                    self._timers_dict['DischargeTimer'].stop()

                    # Store the current supercap voltage as stop voltage
                    self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Current']
                    self._sx5_shell_values_dict['BatteryCharge_%']['Stop'] = self._sx5_shell_values_dict['BatteryCharge_%']['Current']
                    self._sx5_shell_values_dict['BatteryVoltage_mV']['Stop'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Current']

                    # Print Discharge time
                    sys.stdout.write("\033[K")  # Clear to the end of line
                    print(cm.Fore.CYAN + cm.Style.DIM + "- Discharge:")
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• time: {time}s".format(time=self._timers_dict['DischargeTimer'].elapsed_time_s(digits=0)))
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• start voltage: {start}mV".format(start=self._sx5_shell_values_dict['SupercapVoltage_mV']['Start']))
                    print(cm.Fore.CYAN + cm.Style.DIM + "\t• stop voltage: {stop}mV".format(stop=self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop']))

                    # Go to update csv state
                    self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_UPDATE_CSV)
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
            self._csv_log_dict['log_data']['Start Supercap Voltage (Discharge) [mV]'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Start']
            self._csv_log_dict['log_data']['Stop Supercap Voltage (Discharge) [mV]'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop']
            self._csv_log_dict['log_data']['Start Battery Voltage (Discharge) [mV]'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Start']
            self._csv_log_dict['log_data']['Stop Battery Voltage (Discharge) [mV]'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Stop']
            self._csv_log_dict['log_data']['Start Battery Charge (Discharge) [%]'] = self._sx5_shell_values_dict['BatteryCharge_%']['Start']
            self._csv_log_dict['log_data']['Stop Battery Charge (Discharge) [%]'] = self._sx5_shell_values_dict['BatteryCharge_%']['Stop']

            if self._loop_result == "Failed":
                # Update csv log file
                self._csv_log_dict['csv_writer'].writerow(self._csv_log_dict['log_data'].values())

            # Go to relay on state
            self._go_to_next_state(en.LoopTestStateEnum.LT_STATE_ON)

        elif (self._lt_state == en.LoopTestStateEnum.LT_STATE_UPDATE_CSV and
                self._last_lt_state == en.LoopTestStateEnum.LT_STATE_ON):
            # Populate csv dict
            self._csv_log_dict['log_data']['Charge Time [s]'] = self._timers_dict['ChargeTimer'].elapsed_time_s(digits=2)
            self._csv_log_dict['log_data']['Total Time [s]'] = round(self._csv_log_dict['log_data']['Charge Time [s]'] + \
                                                                     self._csv_log_dict['log_data']['Discharge Time [s]'], ndigits=2)
            self._csv_log_dict['log_data']['Start Supercap Voltage (Charge) [mV]'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Start']
            self._csv_log_dict['log_data']['Stop Supercap Voltage (Charge) [mV]'] = self._sx5_shell_values_dict['SupercapVoltage_mV']['Stop']
            self._csv_log_dict['log_data']['Start Battery Voltage (Charge) [mV]'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Start']
            self._csv_log_dict['log_data']['Stop Battery Voltage (Charge) [mV]'] = self._sx5_shell_values_dict['BatteryVoltage_mV']['Stop']
            self._csv_log_dict['log_data']['Start Battery Charge (Charge) [%]'] = self._sx5_shell_values_dict['BatteryCharge_%']['Start']
            self._csv_log_dict['log_data']['Stop Battery Charge (Charge) [%]'] = self._sx5_shell_values_dict['BatteryCharge_%']['Stop']

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
        #self._serial_relay.drive_relay(cmd=en.DebugRelayCommandsEnum.RC_ALL_ON)
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
        self._oscilloscope_init()

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

        # Initialize and start state machine's execution timer
        ex_timer = Timer()
        ex_timer.start()

        while not (self._lt_state == en.LoopTestStateEnum.LT_STATE_STOP and
                   self._last_lt_state == en.LoopTestStateEnum.LT_STATE_STOP):
            # Execute State Machine every 10ms
            if ex_timer.elapsed_time_ms() >= 10:
                
                # Run state machine at the current state
                self._lt_state_machine_manager()

                # Update exit condition
                self._update_exit_condition('Max Num of Loops Reached', (self._current_loop > int(self._lt_config_dict["Loop"]["n_loop"])))
                self._update_exit_condition('Stop Command', (self._lt_cmd == en.LoopTestCommandsEnum.LT_CMD_STOP))

                ex_timer.reset()
            else:
                pass

        # Reset state machine variables
        self._lt_state = en.LoopTestStateEnum.LT_STATE_INIT
        self._last_lt_state = en.LoopTestStateEnum.LT_STATE_INIT

        # Stop sx5 reading thread
        self._thread_dict['sx5_read'].stop()

        # Stop time machine timer
        ex_timer.stop()

        # Update shell and txt log file
        final_result_text = ("\n--- Finished ----", 
                             "-Tot. loops: {loop}".format(loop=self._current_loop-1),
                             "-Elapsed Time: {time} min".format(time=self._timers_dict['GlobalTimer'].elapsed_time_min(digits=2)),
                             "\n-Test finished for: {cause}".format(cause=[key for key, value in self._lt_exit_cause_dict.items() if value]),
                             )

        with open(file=self._txt_log_dict['filename'], mode='a') as file:
            for line in final_result_text: 
                # Print on shell           
                print(cm.Fore.CYAN + cm.Style.DIM + line)

                # Update txt log file
                file.write(f"{line}\n") #TODO check
      
        pass

        print(cm.Fore.CYAN + cm.Style.DIM + "\n*** Exit Loop Test ***")
        print(cm.Fore.CYAN + cm.Style.DIM + "----------------------------------------\n")

    def stop(self):
        """ """
        self._lt_cmd = en.LoopTestCommandsEnum.LT_CMD_STOP
        pass


if __name__ == '__main__':
    test = LoopTest()
    test.init()
    test.run()
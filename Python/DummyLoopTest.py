from SerialRelay import SerialRelay
from Timer import *
from ParseXml import XmlDictConfig
from xml.etree import ElementTree
from Enums import Enums as en
import colorama as cm

class DummyLoopTest(object):

    # ****************************** #
    # ****** Private Methods ******* #
    # ****************************** #
    def __init__(self):
        """"""
        # Global Variables
        self._dlt_config_dict = None
        self._serial_relay = None
        self._global_timer = None
        self._wait_timer = None

        self._relays_toggle_status = None

        self._abs_state_machine_fun_dict = {
            en.DummyLoopTestStateEnum.DLT_STATE_INIT: self._init_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_ON: self._on_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_OFF: self._off_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_WAIT: self._wait_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_STOP: self._stop_state_manager,
        }

        self._dlt_state = None
        self._last_dlt_state = None
        self._dlt_cmd = None
        self._current_loop = None
        self._exit_condition = None

        self._dlt_exit_cause_dict = {
            'Elapsed Time': False,
            'Max Num of Loops Reached': False,
            'Stop Command': False,
        }

    pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def _parse_config_file(self):
        """"""
        self._dlt_config_dict = XmlDictConfig(ElementTree.parse('DLT_Config.xml').getroot())

        pass

    def _serial_relay_init(self):
        """"""
        self._serial_relay = SerialRelay(port=self._dlt_config_dict["Serial"]["com"],
                                         baudrate=self._dlt_config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        self._relays_toggle_status = en.RelayStatusEnum.RS_ON

        pass

    def _timers_init(self):
        """"""
        # Initialize global timer
        self._global_timer = Timer()

        # Initialize wait timer
        self._wait_timer = Timer()

        pass

    # ------------------ STATE MACHINE ------------------ #
    def _go_to_next_state(self, state):
        """"""
        # Store last state
        self._last_dlt_state = self._dlt_state

        # Go to next state
        self._dlt_state = state
        pass

    def _update_exit_condition(self, cause, flag):
        """"""

        # Update the selected cause
        self._dlt_exit_cause_dict[cause] = flag

        # Update the exit condition flag doing the or of all flags
        self._exit_condition = sum(self._dlt_exit_cause_dict.values())

        pass

    def _init_state_manager(self):
        """"""
        # Start timers
        self._global_timer.start()
        self._wait_timer.start()

        # Start from iteration 0
        self._current_loop = 0

        # Go to "On" state manager
        self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_ON
        pass

    def _on_state_manager(self):
        """ Relay On state manager """
        if not self._exit_condition:
            print("\n--- Iteration n°{iter} ---".format(iter=self._current_loop))

            # Put all relay at ON state
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

            # Relay are in on state
            self._relays_toggle_status = en.RelayStatusEnum.RS_ON

            # Save last state
            self._last_dlt_state = self._dlt_state

            # Go to wait state manager
            self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_WAIT

        else:
            # Save last state
            self._last_dlt_state = self._dlt_state
            # Go to stop state
            self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_STOP

        pass

    def _off_state_manager(self):
        """ Relay Off state manager """
        if not self._exit_condition:

            # Put all relay at off state
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_OFF)

            # Relay are in off state
            self._relays_toggle_status = en.RelayStatusEnum.RS_OFF

            # Increase current loop
            self._current_loop += 1

            # Save last state
            self._last_dlt_state = self._dlt_state

            # Go to wait state manager
            self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_WAIT
        else:
            # Save last state
            self._last_dlt_state = self._dlt_state
            # Go to stop state
            self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_STOP

        pass

    def _wait_state_manager(self):
        """"""
        if not self._exit_condition:
            if (self._dlt_state == en.DummyLoopTestStateEnum.DLT_STATE_WAIT and
                (self._last_dlt_state == en.DummyLoopTestStateEnum.DLT_STATE_ON or
                 self._last_dlt_state == en.DummyLoopTestStateEnum.DLT_STATE_OFF)):

                # Restart timer or started if not possible
                try:
                    self._wait_timer.reset()
                except TimerError:
                    self._wait_timer.start()
                # Store the last state
                self._last_dlt_state = self._dlt_state

            elif self._relays_toggle_status == en.RelayStatusEnum.RS_ON:
                if self._wait_timer.elapsed_time_s() >= float(self._dlt_config_dict["Loop"]["time_on_s"]):

                    # Go to "relay off" state manager
                    self._go_to_next_state(en.DummyLoopTestStateEnum.DLT_STATE_OFF)
                else:
                    pass

            elif self._relays_toggle_status == en.RelayStatusEnum.RS_OFF:
                if self._wait_timer.elapsed_time_s() >= float(self._dlt_config_dict["Loop"]["time_off_s"]):

                    # Go to "relay off" state manager
                    self._go_to_next_state(en.DummyLoopTestStateEnum.DLT_STATE_ON)
                else:
                    pass
            else:
                pass
        else:
            # Go to stop state
            self._go_to_next_state(en.DummyLoopTestStateEnum.DLT_STATE_STOP)

        pass

    def _stop_state_manager(self):
        """"""

        # Put off al relays
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

        # Stop Timers
        self._global_timer.stop()
        try:
            self._wait_timer.stop()
        except TimerError:
            pass

        # Close serial relay port
        self._serial_relay.close()

        # Store last state
        self._last_dlt_state = self._dlt_state

        pass

    def _dlt_state_machine_manager(self):
        # Get function from dictionary
        fun = self._abs_state_machine_fun_dict.get(self._dlt_state)

        # Execute function
        fun()
        return

    # ---------------------------------------------------------------- #
    # ------------------------ Public Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def init(self):
        # Initialize Colorama library
        cm.init(autoreset=True)

        # Parse config file
        self._parse_config_file()

        # Initialize timers
        self._timers_init()

        # Initialize serial relay
        self._serial_relay_init()

        # Initialize State Variables
        self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT
        self._dlt_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_INIT

        self._exit_condition = False

        pass

    def run(self):
        """ """
        # Force Command to start
        self._dlt_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_START

        print(cm.Fore.CYAN + cm.Style.DIM + "\n----------------------------------------")
        print(cm.Fore.CYAN + cm.Style.DIM + "*** Run Dummy Loop Test ***")

        while not (self._dlt_state == en.DummyLoopTestStateEnum.DLT_STATE_STOP and
                   self._last_dlt_state == en.DummyLoopTestStateEnum.DLT_STATE_STOP):

            # Run state machine at the current state
            self._dlt_state_machine_manager()

            # Evaluate exit condition
            self._update_exit_condition('Elapsed Time', (self._global_timer.elapsed_time_min() >= float(self._dlt_config_dict["Loop"]["time_test_min"])))
            self._update_exit_condition('Max Num of Loops Reached', (self._current_loop > int(self._dlt_config_dict["Loop"]["n_loop"]) - 1))
            self._update_exit_condition('Stop Command', (self._dlt_cmd == en.DummyLoopTestCommandsEnum.DLT_CMD_STOP))

        # Reset state machine variables
        self._dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT
        self._last_dlt_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT

        print("\n--- Finished ----")
        print("-Tot. loops: {loop}".format(loop=self._current_loop))
        print("-Elapsed Time: {time} min".format(time=self._global_timer.elapsed_time_min()))
        print("\n-Test finished for: {cause}".format(cause=[key for key, value in self._dlt_exit_cause_dict.items() if value]))
        print(cm.Fore.CYAN + cm.Style.DIM + "\n*** Exit Dummy Loop Test ***")
        print(cm.Fore.CYAN + cm.Style.DIM + "----------------------------------------\n")
        pass

    def stop(self):
        """ """
        self._dlt_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_STOP
        pass


if __name__ == '__main__':
    print("--- Create Class ---")
    test = DummyLoopTest()

    print("--- Init ---")
    test.init()

    print("--- Run ---")
    test.run()


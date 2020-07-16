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
        self._wait_time = None

        self._toggle_status = en.RelayStatusEnum.RS_OFF

        self._abs_state_machine_fun_dict = {
            en.DummyLoopTestStateEnum.DLT_STATE_INIT: self._init_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_ON: self._on_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_OFF: self._off_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_WAIT: self._wait_state_manager,
            en.DummyLoopTestStateEnum.DLT_STATE_STOP: self._stop_state_manager,
        }

        self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT
        self._last_abs_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT

        self._abs_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_INIT
        self._last_abs_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_INIT

        self._current_loop = None
        self._exit_condition = False

    pass

    def _parse_config_file(self):
        """"""
        self._dlt_config_dict = XmlDictConfig(ElementTree.parse('DLT_Config.xml').getroot())

        pass

    def _serial_relay_init(self):
          """"""
          self._serial_relay = SerialRelay(port=self._dlt_config_dict["Serial"]["com"],
                                           baudrate=self._dlt_config_dict["Serial"]["baudarate"])
          self._serial_relay.init()
          pass

    def _global_timer_init(self):
        """"""
        self._global_timer = Timer()
        self._global_timer.start()
        pass

    def _init_state_manager(self):
        # Initialize Colorama library
        cm.init(autoreset=True)

        # Initialize global timer
        self._global_timer_init()

        # Initialize and wait timer
        self._wait_timer = Timer()

        # Start from iteration 0
        self._current_loop = 0

        # Go to "On" state manager
        self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_ON
        pass

    def _on_state_manager(self):
        """ Relay On state manager """
        if not self._exit_condition:
            print("\n--- Iteration n°{iter} ---".format(iter=self._current_loop))

            # Put all relay at ON state
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_ON)

            # Relay are in on state
            self._toggle_status = en.RelayStatusEnum.RS_ON

            # Save last state
            self._last_abs_state = self._abs_state

            # Go to wait state manager
            self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_WAIT

        else:
            # Save last state
            self._last_abs_state = self._abs_state
            # Go to stop state
            self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_STOP

        pass

    def _off_state_manager(self):
        """ Relay On state manager """
        if not self._exit_condition:

            # Put all relay at off state
            self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_OFF)

            # Relay are in off state
            self._toggle_status = en.RelayStatusEnum.RS_OFF

            # Increase current loop
            self._current_loop += 1

            # Save last state
            self._last_abs_state = self._abs_state

            # Go to wait state manager
            self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_WAIT
        else:
            # Save last state
            self._last_abs_state = self._abs_state
            # Go to stop state
            self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_STOP

        pass

    def _wait_state_manager(self):
        """"""
        if not self._exit_condition:
            if (self._abs_state == en.DummyLoopTestStateEnum.DLT_STATE_WAIT and
                (self._last_abs_state == en.DummyLoopTestStateEnum.DLT_STATE_ON or
                 self._last_abs_state == en.DummyLoopTestStateEnum.DLT_STATE_OFF)):

                # Restart timer or started if not possible
                try:
                    self._wait_timer.reset()
                except TimerError:
                    self._wait_timer.start()
                # Store the last state
                self._last_abs_state = self._abs_state

            elif self._toggle_status == en.RelayStatusEnum.RS_ON:
                if self._wait_timer.elapsed_time_s >= float(self._dlt_config_dict["Loop"]["time_on_s"]):

                    # Store the last state
                    self._last_abs_state = self._abs_state
                    # Go to "relay off" state manager
                    self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_OFF
                else:
                    pass

            elif self._toggle_status == en.RelayStatusEnum.RS_OFF:
                if self._wait_timer.elapsed_time_s >= float(self._dlt_config_dict["Loop"]["time_off_s"]):

                    # Store the last state
                    self._last_abs_state = self._abs_state
                    # Go to "relay off" state manager
                    self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_ON
                else:
                    pass
            else:
                pass
        else:
            # Save last state
            self._last_abs_state = self._abs_state
            # Go to stop state
            self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_STOP

        pass

    def _stop_state_manager(self):
        """"""

        # Put off al relays
        self._serial_relay.drive_relay(cmd=en.RelayCommandsEnum.RC_ALL_OFF)

        # Stop Timers
        self._global_timer.stop()
        try:
            self._wait_timer.stop()
        except TimerError:
            pass

        # Close serial relay port
        self._serial_relay.close()

        # Store last state
        self._last_abs_state = self._abs_state

        pass

    def _abs_state_machine_manager(self):
        # Get function from dictionary
        fun = self._abs_state_machine_fun_dict.get(self._abs_state)

        # Execute function
        fun()
        return

    # ****************************** #
    # ******* Public Methods ******* #
    # ****************************** #
    def init(self):
        # Parse config file
        self._parse_config_file()

        # Initialize serial relay
        self._serial_relay_init()
        pass

    def start_loop(self):
        """ """
        self._abs_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_START
        self._last_abs_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_START

        print(cm.Fore.CYAN + cm.Style.DIM + "\n----------------------------------------")
        print(cm.Fore.CYAN + cm.Style.DIM + "*** Run Dummy Loop Test ***")

        while not (self._abs_state == en.DummyLoopTestStateEnum.DLT_STATE_STOP and
                   self._last_abs_state == en.DummyLoopTestStateEnum.DLT_STATE_STOP):

            # Run state machine at the current state
            self._abs_state_machine_manager()

            # Evaluate exit condition
            self._exit_condition = (self._global_timer.elapsed_time_hour >= float(self._dlt_config_dict["Loop"]["time_test_hour"]) or
                                    (self._current_loop > int(self._dlt_config_dict["Loop"]["n_loop"]) - 1) or
                                    self._abs_cmd == en.DummyLoopTestCommandsEnum.DLT_CMD_STOP)

        # Reset state machine variables
        self._abs_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT
        self._last_abs_state = en.DummyLoopTestStateEnum.DLT_STATE_INIT

        print("\n--- Finished ----")
        print("-Tot. loops: {loop}".format(loop=self._current_loop))
        print("-Elapsed Time: {time} min".format(time=self._global_timer.elapsed_time_hour))

        print(cm.Fore.CYAN + cm.Style.DIM + "\n*** Exit Dummy Loop Test ***")
        print(cm.Fore.CYAN + cm.Style.DIM + "----------------------------------------\n")
        pass

    def stop_loop(self):
        """ """
        self._abs_cmd = en.DummyLoopTestCommandsEnum.DLT_CMD_STOP
        pass


if __name__ == '__main__':
    print("--- Create Class ---")
    test = DummyLoopTest()

    print("--- Init ---")
    test.init()

    print("--- Run ---")
    test.start_loop()


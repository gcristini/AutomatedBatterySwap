from Enums import Enums as en
import colorama as cm
from SerialRelay import SerialRelay
from ParseXml import XmlDictConfig
from xml.etree import ElementTree


class DebugTest(object):
    def __init__(self):
        """"""
        self._debug_test_fun_dict = {
            en.DebugTestStateEnum.DT_STATE_INIT: self._init_state_manager,
            en.DebugTestStateEnum.DT_STATE_WAIT_CMD: self._wait_cmd_state_manager,
            en.DebugTestStateEnum.DT_STATE_EXIT: self._exit_state_manager,
        }

        self._serial_relay = None
        self._config_dict = None

        self._debug_test_state = None
        self._last_debug_test_state = None

        pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    @staticmethod
    def _print_help():
        """"""
        print(cm.Fore.YELLOW + cm.Style.DIM + "\n----------------------------------------")
        print(cm.Fore.YELLOW + cm.Style.DIM + "--- Debug Test: Show Usage ---")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-{relay}_{state}")
        print(cm.Fore.YELLOW + cm.Style.DIM + "\trelay: [packp, sda, scl, detect, ntc, all]")
        print(cm.Fore.YELLOW + cm.Style.DIM + "\tstate: [on, off]")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-exit: exit from debug mode")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-help: this help")
        print(cm.Fore.YELLOW + cm.Style.DIM + "----------------------------------------\n")
        pass

    def _parse_config_file(self):
        """"""
        self._config_dict = XmlDictConfig(ElementTree.parse('DBG_Config.xml').getroot())

        pass

    def _serial_relay_init(self):
        """"""
        self._serial_relay = SerialRelay(port=self._config_dict["Serial"]["com"],
                                         baudrate=self._config_dict["Serial"]["baudarate"])
        self._serial_relay.init()
        pass

    # ------------------ STATE MACHINE ------------------ #
    def _store_last_state(self):
        """"""
        self._last_debug_test_state = self._debug_test_state
        pass

    def _init_state_manager(self):
        """"""
        print(cm.Fore.RED + cm.Style.DIM + "----------------------------------------")
        print(cm.Fore.RED + cm.Style.DIM + "*** Debug Mode ***\n")

        # Store last state
        self._store_last_state()

        # Go to wait command state
        self._debug_test_state = en.DebugTestStateEnum.DT_STATE_WAIT_CMD

        return

    def _wait_cmd_state_manager(self):
        """"""
        # Wait for command
        cmd = input(cm.Fore.RED + cm.Style.DIM + "- Please enter a command: ")

        if cmd in en.DebugTestCommandsEnum.values():
            if cmd == en.MainAppCommandsEnum.MA_CMD_EXIT:
                # Store last state
                self._store_last_state()

                # Go to exit state
                self._debug_test_state = en.DebugTestStateEnum.DT_STATE_EXIT

            elif cmd == en.MainAppCommandsEnum.MA_CMD_HELP:
                # Print Debug Help
                self._print_help()

            else:
                print(cm.Fore.RED + cm.Style.DIM + "--- Wrong command: use a valid command!")
                self._print_help()

        elif cmd in en.RelayCommandsEnum.values():
            # Drive relay
            self._serial_relay.drive_relay(cmd)

        else:
            print(cm.Fore.RED + cm.Style.DIM + "--- Wrong command: use a valid command!")
            self._print_help()
        pass

    def _exit_state_manager(self):
        """"""
        print(cm.Fore.RED + cm.Style.DIM + "\n*** Exit Debug Mode ***")
        print(cm.Fore.RED + cm.Style.DIM + "----------------------------------------\n")

        # Close serial relay port
        self._serial_relay.close()

        # Store last state
        self._store_last_state()
        pass

    def _debug_test_state_manager(self):
        """"""
        # Get function from Dictionary
        fun = self._debug_test_fun_dict.get(self._debug_test_state)

        # Execute function
        fun()

        return

    # ---------------------------------------------------------------- #
    # ------------------------ Public Methods ------------------------ #
    # ---------------------------------------------------------------- #
    def init(self):
        """"""
        # Initialize Colorama library
        cm.init(autoreset=True)

        # Parse config file
        self._parse_config_file()

        # Initialize serial relay
        self._serial_relay_init()

        # Initialize state machine states
        self._debug_test_state = en.DebugTestStateEnum.DT_STATE_INIT

        pass

    def run(self):
        """ """
        while not (self._debug_test_state == en.DebugTestStateEnum.DT_STATE_EXIT and
                   self._last_debug_test_state == en.DebugTestStateEnum.DT_STATE_EXIT):
            self._debug_test_state_manager()

        pass


if __name__ == '__main__':
    test = DebugTest()
    test.init()
    test.run()

from AutomatedBatterySwap import AutomatedBatterySwap
from Enums import Enums as en
import colorama as cm

class Main(object):

    def __init__(self):
        """"""
        self._abs = None

        self._main_app_fun_dict= {
            en.MainAppStateEnum.MA_STATE_INIT: self._init_state_manager,
            en.MainAppStateEnum.MA_STATE_WAIT_CMD: self._wait_cmd_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_LOOP: self._run_loop_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_DEBUG: self._run_debug_state_manager,
            en.MainAppStateEnum.MA_STATE_EXIT: self._exit_state_manager
        }

        self._main_app_state = None
        self._last_main_app_state = None

    pass

    def _init_state_manager(self):
        """"""
        # Instantiate AutomatedBatterySwap
        self._abs = AutomatedBatterySwap()

        # Initialize AutomatedBatterySwap
        self._abs.init()

        print(cm.Fore.GREEN + " ---------- WELCOME TO AUTOMATED BATTERY SWAP APP! ---------- ")

        # Store last state
        self._last_main_app_state = self._main_app_state

        # Go to "Wait Command state"
        self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
        pass

    def _wait_cmd_state_manager(self):
        """"""
        cmd = input("Please enter a command: ")
        print('\n')

        if cmd == en.MainAppCommandsEnum.MA_CMD_RUN_LOOP:
            # Store last state
            self._last_main_app_state = self._main_app_state

            # Go to "run loop" state
            self._main_app_state = en.MainAppStateEnum.MA_STATE_RUN_LOOP

        elif cmd == en.MainAppCommandsEnum.MA_CMD_RUN_DEBUG:
            # Store last state
            self._last_main_app_state = self._main_app_state

            # Go to "run loop" state
            self._main_app_state = en.MainAppStateEnum.MA_STATE_RUN_DEBUG

    pass

    def _run_loop_state_manager(self):
        """"""
        print(cm.Fore.CYAN + cm.Style.DIM + "Run Loop Test\n")
        # Start loop test
        self._abs.start_loop()

        # Store last state
        self._last_main_app_state = self._main_app_state

        # Go to "Wait Command" state
        self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
        pass

    def _run_debug_state_manager(self):
        """"""
        # Transition to the state...
        if (self._main_app_state == en.MainAppStateEnum.MA_STATE_RUN_DEBUG and
                self._last_main_app_state != en.MainAppStateEnum.MA_STATE_RUN_DEBUG):
            print(cm.Fore.CYAN + cm.Style.DIM + "Debug Mode\n")

            # Store last state
            self._last_main_app_state = self._main_app_state
        else:
            cmd = input("Debug: Please enter a command: ")

            # Check if command is valid and relative to main app
            if cmd in en.MainAppCommandsEnum.values():
                if cmd == en.MainAppCommandsEnum.MA_CMD_EXIT:
                    print(cm.Fore.CYAN + cm.Style.DIM + "Exit Debug Mode\n")

                    # Go to "Wait command" state
                    self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
                elif cmd == en.MainAppCommandsEnum.MA_CMD_HELP:
                    print ("HELP USAGE TODO") #TODO
                else:
                    print("Wrong command: use a valid command")

            # Check if command is valid and is a relay command
            elif cmd in en.RelayCommandsEnum.values():
                # Drive relay
                self._abs.drive_relay(cmd)
            else:
                print("Wrong command: use a valid command")

        pass

    def _exit_state_manager(self):
        """"""
        pass

    def _main_app_state_machine_manager(self):
        # Get Function from Dictionary
        fun = self._main_app_fun_dict.get(self._main_app_state)

        # Execute function
        fun()

        return

    def init(self):
        """"""
        # Initialize Colorama library
        cm.init(autoreset=True)

        # Initialize state machine states
        self._main_app_state = en.MainAppStateEnum.MA_STATE_INIT
        self._last_main_app_state = en.MainAppStateEnum.MA_STATE_INIT

        pass

    def run(self):

        while True:
            self._main_app_state_machine_manager()



if __name__ == "__main__":
    test = Main()
    test.init()
    test.run()
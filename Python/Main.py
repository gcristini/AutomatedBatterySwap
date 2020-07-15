from AutomatedBatterySwap import AutomatedBatterySwap
from Enums import Enums as en
import colorama as cm
from CustomThread import CustomThread

class Main(object):

    def __init__(self):
        """"""
        self._abs = None

        self._main_app_fun_dict = {
            en.MainAppStateEnum.MA_STATE_INIT: self._init_state_manager,
            en.MainAppStateEnum.MA_STATE_WAIT_CMD: self._wait_cmd_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_LOOP: self._run_loop_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_DEBUG: self._run_debug_state_manager,
            en.MainAppStateEnum.MA_STATE_EXIT: self._exit_state_manager
        }

        self._main_app_state = None
        self._last_main_app_state = None

        self._abs_thread = None

    pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    @staticmethod
    def _print_help(mode):
        """"""
        if mode == "main":
            print(cm.Fore.YELLOW + cm.Style.DIM + "\n----------------------------")
            print(cm.Fore.YELLOW + cm.Style.DIM + "--- Show Usage ---")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-loop: run loop test")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-debug: enter in debug mode")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-exit: exit from script")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-help: this help")
            print(cm.Fore.YELLOW + cm.Style.DIM + "----------------------------\n")
        elif mode == "debug":
            print(cm.Fore.YELLOW + cm.Style.DIM + "\n----------------------------")
            print(cm.Fore.YELLOW + cm.Style.DIM + "--- Show Usage ---")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-{relay}_{state}")
            print(cm.Fore.YELLOW + cm.Style.DIM + "\trelay: [packp, sda, scl, detect, ntc, all]")
            print(cm.Fore.YELLOW + cm.Style.DIM + "\tstate: [on, off]")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-exit: exit from debug mode")
            print(cm.Fore.YELLOW + cm.Style.DIM + "-help: this help")
            print(cm.Fore.YELLOW + cm.Style.DIM + "----------------------------\n")

        else:
            pass
        pass

    # ------ STATE MACHINE ------ #
    def _store_last_state(self):
        """"""
        self._last_main_app_state = self._main_app_state
        pass

    def _init_state_manager(self):
        """"""
        # Instantiate AutomatedBatterySwap
        self._abs = AutomatedBatterySwap()

        # Initialize AutomatedBatterySwap
        self._abs.init()

        # Create thread that run loop test function
        self._abs_thread = CustomThread(runnable=self._abs.start_loop)

        print(cm.Fore.MAGENTA + " ---------- WELCOME TO AUTOMATED BATTERY SWAP APP! ---------- ")

        # Store last state
        self._store_last_state()

        # Go to "Wait Command state"
        self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
        pass

    def _wait_cmd_state_manager(self):
        """"""
        cmd = input("- Please enter a command: ")

        if cmd == en.MainAppCommandsEnum.MA_CMD_RUN_LOOP:
            # Store last state
            self._store_last_state()

            # Go to "run loop" state
            self._main_app_state = en.MainAppStateEnum.MA_STATE_RUN_LOOP

        elif cmd == en.MainAppCommandsEnum.MA_CMD_RUN_DEBUG:
            # Store last state
            self._store_last_state()

            # Go to "run loop" state
            self._main_app_state = en.MainAppStateEnum.MA_STATE_RUN_DEBUG

        elif cmd == en.MainAppCommandsEnum.MA_CMD_HELP:
            self._print_help("main")

        elif cmd == en.MainAppCommandsEnum.MA_CMD_EXIT:
            # Store last state
            self._store_last_state()

            # Go to "exit" state
            self._main_app_state = en.MainAppStateEnum.MA_STATE_EXIT

        else:
            print("--- Wrong command: use a valid command!")
            self._print_help("main")

    pass

    def _run_loop_state_manager(self):
        """"""
        print(cm.Fore.CYAN + cm.Style.DIM + "\n---------------------")
        print(cm.Fore.CYAN + cm.Style.DIM + "*** Run Loop Test ***")

        # Run thread
        try:
            # Start loop test thread
            self._abs_thread.start()
        except RuntimeError:
            self._abs_thread = CustomThread(runnable=self._abs.start_loop)
            self._abs_thread.start()

        # You can stop the loop with a stop command
        while self._abs_thread.is_alive():
            stop_cmd = input()
            if stop_cmd == en.MainAppCommandsEnum.MA_CMD_STOP:
                self._abs.stop_loop()
                break
            else:
                pass

            pass

        self._abs_thread.join()

        # Store last state
        self._store_last_state()

        print(cm.Fore.CYAN + cm.Style.DIM + "*** Exit Loop Test ***")
        print(cm.Fore.CYAN + cm.Style.DIM + "----------------------------------------\n")

        # Go to "Wait Command" state
        self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
        pass

    def _run_debug_state_manager(self):
        """"""
        # Transition to the state...
        if (self._main_app_state == en.MainAppStateEnum.MA_STATE_RUN_DEBUG and
                self._last_main_app_state != en.MainAppStateEnum.MA_STATE_RUN_DEBUG):
            print(cm.Fore.RED + cm.Style.DIM + "------------------")
            print(cm.Fore.RED + cm.Style.DIM + "*** Debug Mode ***\n")

            # Store last state
            self._store_last_state()
        else:
            # Wait for command
            cmd = input(cm.Fore.RED + cm.Style.DIM + "- Please enter a command: ")

            # Check if command is valid and relative to main app
            if cmd in en.MainAppCommandsEnum.values():
                if cmd == en.MainAppCommandsEnum.MA_CMD_EXIT:
                    print(cm.Fore.RED + cm.Style.DIM + "\n*** Exit Debug Mode ***")
                    print(cm.Fore.RED + cm.Style.DIM + "----------------------------------------\n")

                    # self._store_last_state()
                    # Go to "Wait command" state
                    self._main_app_state = en.MainAppStateEnum.MA_STATE_WAIT_CMD
                elif cmd == en.MainAppCommandsEnum.MA_CMD_HELP:
                    # Print Debug Help
                    self._print_help("debug")  # TODO
                else:
                    print(cm.Fore.RED + cm.Style.DIM + "--- Wrong command: use a valid command!\n")
                    self._print_help("debug")

            # Check if command is valid and is a relay command
            elif cmd in en.RelayCommandsEnum.values():
                # Drive relay
                self._abs.drive_relay(cmd)
            else:
                print(cm.Fore.RED + cm.Style.DIM + "--- Wrong command: use a valid command!\n")

        pass

    def _exit_state_manager(self):
        """"""
        print(cm.Fore.MAGENTA + " \n---------- GOODBYE!!! ---------- ")

        # Store last state
        self._store_last_state()

        pass

    def _main_app_state_machine_manager(self):
        # Get Function from Dictionary
        fun = self._main_app_fun_dict.get(self._main_app_state)

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

        # Initialize state machine states
        self._main_app_state = en.MainAppStateEnum.MA_STATE_INIT
        self._last_main_app_state = en.MainAppStateEnum.MA_STATE_INIT

        pass

    def run(self):

        while not (self._main_app_state == en.MainAppStateEnum.MA_STATE_EXIT and
                   self._last_main_app_state == en.MainAppStateEnum.MA_STATE_EXIT):
            self._main_app_state_machine_manager()


if __name__ == "__main__":
    test = Main()
    test.init()
    test.run()

from DummyLoopTest import DummyLoopTest
from LoopTest import LoopTest
from DebugTest import DebugTest
from Enums import Enums as en
import colorama as cm
from CustomThread import CustomThread


class Main(object):

    def __init__(self):
        """"""
        # State Machine
        self._main_app_fun_dict = {
            en.MainAppStateEnum.MA_STATE_INIT: self._init_state_manager,
            en.MainAppStateEnum.MA_STATE_WAIT_CMD: self._wait_cmd_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_LOOP: self._run_loop_state_manager,
            en.MainAppStateEnum.MA_STATE_RUN_DEBUG: self._run_debug_state_manager,
            en.MainAppStateEnum.MA_STATE_EXIT: self._exit_state_manager
        }
        self._main_app_state = None
        self._last_main_app_state = None

        self._loop = None
        self._loop_thread = None

        # Debug Test
        self._dbgt = None

    pass

    # ---------------------------------------------------------------- #
    # ----------------------- Private Methods ------------------------ #
    # ---------------------------------------------------------------- #
    @staticmethod
    def _print_help():
        """"""       
        print(cm.Fore.YELLOW + cm.Style.DIM + "\n----------------------------------------")
        print(cm.Fore.YELLOW + cm.Style.DIM + "--- Main: Show Usage ---")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-loop: run loop test")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-dummy_loop: run dummy_loop test")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-debug: enter in debug mode")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-exit: exit from script")
        print(cm.Fore.YELLOW + cm.Style.DIM + "-help: this help")
        print(cm.Fore.YELLOW + cm.Style.DIM + "----------------------------------------\n")
       
        pass

    def _init_dummy_loop_test(self):
        """"""
        # Instantiate DummyLoopTest
        self._loop = DummyLoopTest()

        # Initialize DummyLoopTest
        self._loop.init()

        # Create thread that run loop test function
        self._loop_thread = CustomThread(runnable=self._loop.run)

        pass

    def _init_smart_loop_test(self):
        # Instantiate SmartLoopTest
        self._loop = LoopTest()

        # Initialize SmartLoopTest
        self._loop.init()

        # Create thread that run loop test function
        self._loop_thread = CustomThread(runnable=self._loop.run)

        pass

    def _init_debug_test(self):
        # Instantiate DebugTest
        self._dbgt = DebugTest()

        # Initialize DebugTest
        self._dbgt.init()
        pass

    # ------------------ STATE MACHINE ------------------ #
    def _go_to_next_state(self, state):
        """"""
        # Store last state
        self._last_main_app_state = self._main_app_state

        # Go to next state
        self._main_app_state = state
        pass

    def _init_state_manager(self):
        """"""
        print(cm.Fore.MAGENTA + " ------------------------------------------------------------ ")
        print(cm.Fore.MAGENTA + " ---------- WELCOME TO AUTOMATED BATTERY SWAP APP! ---------- ")
        print(cm.Fore.MAGENTA + " ------------------------------------------------------------ ")

        # Go to "Wait Command state"
        self._go_to_next_state(en.MainAppStateEnum.MA_STATE_WAIT_CMD)
        pass

    def _wait_cmd_state_manager(self):
        """"""
        cmd = input("- Please enter a command: ")

        if cmd == en.MainAppCommandsEnum.MA_CMD_RUN_DUMMY_LOOP:
            # Initialize Dummy Loop test
            self._init_dummy_loop_test()

            # Go to "run loop" state
            self._go_to_next_state(en.MainAppStateEnum.MA_STATE_RUN_LOOP)

        elif cmd == en.MainAppCommandsEnum.MA_CMD_RUN_SMART_LOOP:
            # Initialize Smart Loop test
            self._init_smart_loop_test()

            # Go to "run loop" state
            self._go_to_next_state(en.MainAppStateEnum.MA_STATE_RUN_LOOP)

        elif cmd == en.MainAppCommandsEnum.MA_CMD_RUN_DEBUG:
            # Initialize Debug test
            self._init_debug_test()

            # Go to "run loop" state
            self._go_to_next_state(en.MainAppStateEnum.MA_STATE_RUN_DEBUG)

        elif cmd == en.MainAppCommandsEnum.MA_CMD_HELP:
            self._print_help()

        elif cmd == en.MainAppCommandsEnum.MA_CMD_EXIT:

            # Go to "exit" state
            self._go_to_next_state(en.MainAppStateEnum.MA_STATE_EXIT)

        else:
            print("--- Wrong command: use a valid command!")
            self._print_help()

    pass

    def _run_loop_state_manager(self):
        """ Run loop: this state is valid both dummy and smart loop test because routines are similar """
        # Run thread
        input("\nPress a key to start test...")

        try:
            # Start loop test thread
            self._loop_thread.start()
        except RuntimeError:
            self._loop_thread = CustomThread(runnable=self._loop.run)
            self._loop_thread.start()

        # You can stop the loop with a stop command
        while self._loop_thread.is_alive():
            stop_cmd = input()
            if stop_cmd == en.MainAppCommandsEnum.MA_CMD_STOP:
                self._loop.stop()
                break
            else:
                pass

            pass

        # Waiting for thread end
        self._loop_thread.join()

        # Go to "Wait Command" state
        self._go_to_next_state(en.MainAppStateEnum.MA_STATE_WAIT_CMD)
        pass

    def _run_debug_state_manager(self):
        """"""
        # Run debug test script
        self._dbgt.run()

        # Go to "Wait Command" state
        self._go_to_next_state(en.MainAppStateEnum.MA_STATE_WAIT_CMD)

        pass

    def _exit_state_manager(self):
        """"""
        print(cm.Fore.MAGENTA + "\n -------------------------------- ")
        print(cm.Fore.MAGENTA + " ---------- GOODBYE!!! ---------- ")
        print(cm.Fore.MAGENTA + " -------------------------------- ")

        # Store last state
        self._last_main_app_state = self._main_app_state

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
        """"""
        while not (self._main_app_state == en.MainAppStateEnum.MA_STATE_EXIT and
                   self._last_main_app_state == en.MainAppStateEnum.MA_STATE_EXIT):
            self._main_app_state_machine_manager()


if __name__ == "__main__":
    test = Main()
    test.init()
    test.run()

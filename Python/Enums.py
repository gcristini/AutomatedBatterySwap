class Enums:
    """    """
    class RelayStatusEnum:
        RS_ON = "on"
        RS_OFF = "off"

    # ------------------------------ #
    # -------- LoopTest.py --------- #
    # ------------------------------ #
    class LoopTestStateEnum:
        LT_STATE_INIT = "init"
        LT_STATE_ON = "on"
        LT_STATE_OFF = "off"
        LT_STATE_UPDATE_CSV = "update_csv"
        LT_STATE_STOP = "stop"

    class LoopTestCommandsEnum:
        LT_CMD_INIT = "init"
        LT_CMD_START = "start"
        LT_CMD_STOP = "stop"

    class RelaysEnum:
        # Relay commands
        RELAY_PACK_P = "packp"
        RELAY_SCL = "scl"
        RELAY_SDA = "sda"
        RELAY_DETECT = "detect"
        RELAY_NTC = "ntc"

        @classmethod
        def vars(cls):
            return [name for name, value in vars(cls).items() if name.isupper()]

        @classmethod
        def values(cls):
            return [value for name, value in vars(cls).items() if name.isupper()]

    # ------------------------------ #
    # ------ DummyLoopTest.py ------ #
    # ------------------------------ #
    class DummyLoopTestStateEnum:
        DLT_STATE_INIT = "init"
        DLT_STATE_ON = "on"
        DLT_STATE_OFF = "off"
        DLT_STATE_WAIT = "wait"
        DLT_STATE_STOP = "stop"

    class DummyLoopTestCommandsEnum:
        DLT_CMD_INIT = "init"
        DLT_CMD_START = "start"
        DLT_CMD_STOP = "stop"

    # ------------------------------ #
    # -------- DebugTest.py -------- #
    # ------------------------------ #
    class DebugTestStateEnum:
        DT_STATE_INIT = "init"
        DT_STATE_WAIT_CMD = "wait_cmd"
        DT_STATE_EXIT = "exit"

    class DebugTestCommandsEnum:
        DT_CMD_EXIT = "exit"
        DT_CMD_HELP = "help"

        @classmethod
        def vars(cls):
            return [name for name, value in vars(cls).items() if name.isupper()]

        @classmethod
        def values(cls):
            return [value for name, value in vars(cls).items() if name.isupper()]

    class DebugRelayCommandsEnum:
        # Relay commands
        RC_PACK_P_ON = "packp on"
        RC_PACK_P_OFF = "packp off"
        RC_SCL_ON = "scl on"
        RC_SCL_OFF = "scl off"
        RC_SDA_ON = "sda on"
        RC_SDA_OFF = "sda off"
        RC_DETECT_ON = "detect on"
        RC_DETECT_OFF = "detect off"
        RC_NTC_ON = "ntc on"
        RC_NTC_OFF = "ntc off"
        RC_ALL_ON = "all on"
        RC_ALL_OFF = "all off"

        @classmethod
        def vars(cls):
            return [name for name, value in vars(cls).items() if name.isupper()]

        @classmethod
        def values(cls):
            return [value for name, value in vars(cls).items() if name.isupper()]

    # ------------------------------ #
    # ---------- Main.py ----------- #
    # ------------------------------ #
    class MainAppStateEnum:
        MA_STATE_INIT = "init"
        MA_STATE_WAIT_CMD = "wait_cmd"
        MA_STATE_RUN_LOOP = "run_loop"
        MA_STATE_RUN_DEBUG = "run_debug"
        MA_STATE_STOP = "stop"
        MA_STATE_HELP = "help"
        MA_STATE_EXIT = "exit"

    class MainAppCommandsEnum:
        # State machine Commands
        MA_CMD_RUN_SMART_LOOP = "loop"
        MA_CMD_RUN_DUMMY_LOOP = "dummy_loop"
        MA_CMD_RUN_DEBUG = "debug"
        MA_CMD_STOP = "stop"
        MA_CMD_EXIT = "exit"
        MA_CMD_HELP = "help"

        @classmethod
        def vars(cls):
            return [name for name, value in vars(cls).items() if name.isupper()]

        @classmethod
        def values(cls):
            return [value for name, value in vars(cls).items() if name.isupper()]


if __name__ == "__main__":
    test = Enums.MainAppCommandsEnum
    print(type(test.vars()))
    print(test.vars())
    print(test.values())

    print ("exit" in test.values())
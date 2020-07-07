class Enums:
    """    """
    class RelayStatusEnum:
        RS_ON = "on"
        RS_OFF = "off"

    class BatterySwapStateEnum:
        ABS_STATE_INIT = "init"
        ABS_STATE_ON = "on"
        ABS_STATE_OFF = "off"
        ABS_STATE_WAIT = "wait"
        ABS_STATE_STOP = "stop"
        ABS_STATE_MAX_ENUM = ""

    class BatterySwapCommandsEnum:
        ABS_CMD_INIT = "init"
        ABS_CMD_START = "start"
        ABS_CMD_STOP = "stop"
        ABS_CMD_MAX_ENUM = ""

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
        MA_CMD_RUN_LOOP = "loop"
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

    class RelayCommandsEnum:
        # Relay commands
        RC_PACK_P_ON = "packp_on"
        RC_PACK_P_OFF = "packp_off"
        RC_SDA_ON = "sda_on"
        RC_SDA_OFF = "sda_off"
        RC_SCL_ON = "scl_on"
        RC_SCL_OFF = "scl_off"
        RC_DETECT_ON = "detect_on"
        RC_DETECT_OFF = "detect_off"
        RC_NTC_ON = "ntc_on"
        RC_NTC_OFF = "ntc_off"
        RC_ALL_ON = "all_on"
        RC_ALL_OFF = "all_off"

        @classmethod
        def vars(cls):
            return [name for name, value in vars(cls).items() if name.isupper()]

        @classmethod
        def values(cls):
            return [value for name, value in vars(cls).items() if name.isupper()]


if __name__ == "__main__":
    test=Enums.MainAppCommandsEnum
    print (type(test.vars()))
    print (test.vars())
    print (test.values())

    print ("exit" in test.values())
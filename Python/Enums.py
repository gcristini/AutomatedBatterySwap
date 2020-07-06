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
        MAS_INIT = "init"
        MAS_WAIT = "wait"
        MAS_RUN = "run"
        MAS_STOP = "stop"
        MAS_HELP = "help"
        MAS_EXIT = "exit"
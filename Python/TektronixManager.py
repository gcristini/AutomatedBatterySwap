import pyvisa

class OscilloscopeManager(object):
    def __init__(self):
        pass

    def connect(self):
        self


if __name__ == '__main__':



rm=pyvisa.ResourceManager()
print (rm.list_resources())
inst=rm.open_resource("USB0::0x0699::0x03A4::C040407::INSTR")
print(inst.query("*IDN?"))
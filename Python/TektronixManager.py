import pyvisa


class TektronixManager(object):
    def __init__(self, resource_name="", image_name=""):
        self._oscilloscope = pyvisa.ResourceManager
        self._config_dict = None
        self._resource_name = resource_name
        self._image_name = image_name

        pass

    def _connect(self):
        try:
            resource = pyvisa.ResourceManager()
            self._oscilloscope = resource.open_resource(self._resource_name)
        except pyvisa.errors.VisaIOError:
            print(" Oscilloscope not found. Available resources:")
            for item in resource.list_resources():
                print(item)

        idn = self._oscilloscope.query("*IDN?").split(",")
        if idn:
            print("- {osc} {mod} connected".format(osc=idn[0], mod=idn[1]))
        else:
            pass


    def init(self):
        self._connect()

    def save_screen(self):
        self._oscilloscope.write('SAVe:IMAGe "{name}.bmp"'.format(name=self._image_name))

    @property
    def image_name(self):
        return self._image_name

    @image_name.setter
    def image_name(self, name):
        self._image_name=name


if __name__ == '__main__':
    test=TektronixManager(resource_name="USB0::0x0699::0x03A4::C040407::INSTR", image_name="tiziocaio")
    test.init()
    print ("save the first")
    test.save_screen()


    test.image_name="tuasorella"
    print("save the second")
    test.save_screen()
    print ("end")

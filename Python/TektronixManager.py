import pyvisa


class TektronixManager(object):
    def __init__(self, resource_name="", image_name="", images_dir=""):
        """"""
        self._oscilloscope = pyvisa.ResourceManager
        self._config_dict = None
        self._resource = None
        self._resource_name = resource_name
        self._images_dir = images_dir
        self._image_name = image_name

        pass

    def connect(self):
        try:
            self._resource = pyvisa.ResourceManager()
            self._oscilloscope = self._resource.open_resource(self._resource_name)

        except pyvisa.errors.VisaIOError:
            print(" Oscilloscope not found. Available resources:")
            for item in self._resource.list_resources():
                print(item)

        idn = self._oscilloscope.query("*IDN?").split(",")

        if idn:
            print("- {osc} {mod} connected".format(osc=idn[0], mod=idn[1]))
        else:
            pass

    def disconnect(self):
        """"""
        self._resource.close()

        pass

    def init(self):
        """"""
        # Connect oscilloscope
        self.connect()

        # Create the save directory
        self.create_dir(self._images_dir)
        pass

    def create_dir(self, dir):
        """"""
        # Update save directory field
        self._images_dir = dir

        # Create directory
        self._oscilloscope.write('FILESystem:MKDir "{dir}"'.format(dir=self._images_dir))
        pass

    def save_screen(self):
        """"""
        self._oscilloscope.write('SAVe:IMAGe "{dir}/{name}.bmp"'.format(dir=self._images_dir,
                                                                        name=self._image_name))
        pass

    @property
    def image_name(self):
        return self._image_name

    @image_name.setter
    def image_name(self, name):
        self._image_name = name


if __name__ == '__main__':
    test = TektronixManager(resource_name="USB0::0x0699::0x0413::C013693::INSTR",
                            image_name="pluto",
                            images_dir="pippo")
    test.init()
    test.save_screen()

    test.disconnect()



import pyvisa
from Libraries.Enums import Enums

class TektronixManager(object):
    def __init__(self, resource_name="", image_name="", images_dir=""):
        """"""
        self._oscilloscope = pyvisa.ResourceManager
        self._config_dict = None
        self._resource = None
        self._resource_name = resource_name
        self._images_dir = images_dir
        self._image_name = image_name
        self._resource_state = None

        pass

    def connect(self):
        if self._resource_state != Enums.TektronixManagerStateEnum.TM_STATE_OPEN:
            try:
                self._resource = pyvisa.ResourceManager()
                self._oscilloscope = self._resource.open_resource(self._resource_name)

            except pyvisa.errors.VisaIOError:
                print("- Oscilloscope not found. Available resources:")
                #for item in self._resource.list_resources():
                #    print(item)
                print ("  {list}".format(list=list(self._resource.list_resources())))
            else:
                # Get resource idn to check connection
                idn = self._oscilloscope.query("*IDN?").split(",")

                if idn:
                    print("- {osc} {mod} connected".format(osc=idn[0], mod=idn[1]))
                    self._resource_state = Enums.TektronixManagerStateEnum.TM_STATE_OPEN
                else:
                    pass

    def disconnect(self):
        """"""
        if self._resource_state == Enums.TektronixManagerStateEnum.TM_STATE_OPEN:
            # Close resource
            self._resource.close()

            # Update resource state
            self._resource_state = Enums.TektronixManagerStateEnum.TM_STATE_CLOSE

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
        if self._resource_state == Enums.TektronixManagerStateEnum.TM_STATE_OPEN:
            # Update save directory field
            self._images_dir = dir

            # Create directory
            self._oscilloscope.write('FILESystem:MKDir "{dir}"'.format(dir=self._images_dir))
        else:
            pass
        pass

    def save_screen(self):
        """"""
        if self._resource_state == Enums.TektronixManagerStateEnum.TM_STATE_OPEN:
            # Save image in the specified directory
            self._oscilloscope.write('SAVe:IMAGe "{dir}/{name}.bmp"'.format(dir=self._images_dir,
                                                                            name=self._image_name))
        else:
            pass

        pass

    @property
    def image_name(self):
        return self._image_name

    @image_name.setter
    def image_name(self, name):
        self._image_name = name

    @property
    def resource_state(self):
        return self._resource_state


if __name__ == '__main__':
    test = TektronixManager(resource_name="USB0::0x0699::0x0413::C013693::INSTR",
                            image_name="pluto",
                            images_dir="pippo")
    test.init()
    test.save_screen()

    test.disconnect()



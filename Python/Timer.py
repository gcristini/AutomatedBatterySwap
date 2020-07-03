# timer.py

import time

class TimerError(Exception):
    """A custom exception used to report errors in use of Timer class"""

class Timer:
    def __init__(self):
        self._start_time = None

    def start(self):
        """Start a new timer"""
        if self._start_time is not None:
            raise TimerError(f"Timer is running. Use .stop() to stop it")

        self._start_time = time.perf_counter()

    def stop(self):
        """Stop the timer, and report the elapsed time"""
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")

        elapsed_time = time.perf_counter() - self._start_time
        self._start_time = None
        print(f"Elapsed time: {elapsed_time:0.4f} seconds")

    def reset(self):
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")
        self._start_time = time.perf_counter()

    @property
    def elapse_time_min(self):
        return (time.perf_counter() - self._start_time)/60

    @property
    def elapsed_time_s(self):
        return time.perf_counter() - self._start_time

    @property
    def elapsed_time_ms(self):
        return (time.perf_counter() - self._start_time)*1000

    @property
    def elapsed_time_us(self):
        return (time.perf_counter() - self._start_time)*1000000


if __name__ == '__main__':
    # Init and start timer
    test_timer = Timer()
    test_timer.start()

    # Pause 2 seconds
    time.sleep(2)
    print("1) Timer elapsed: {time}".format(time=test_timer.elapsed_time_s))

    print("--- Reset Timer...")

    #Reset timer and wait 5 second
    test_timer.reset()
    time.sleep(5)

    print("2) Timer elapsed: {time}".format(time=test_timer.elapsed_time_s))

    test_timer.stop()

    print ("Finished")


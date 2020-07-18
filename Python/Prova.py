from Timer import Timer
import time
timer_dict = {
    'timer1': Timer,
    'timer2': Timer,
    'timer3': Timer,
}


print (list(timer_dict))
#dizionario = Timer()

for timer in list(timer_dict):
    timer_dict[timer] = Timer()


for timer in list(timer_dict):
    timer_dict[timer].start()


time.sleep(1)

for timer in list(timer_dict):
    timer_dict[timer].stop()

for timer in list(timer_dict):
    print(timer_dict[timer].elapsed_time_ms())


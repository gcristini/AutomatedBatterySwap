import csv
csv_log_dict = {
            #'Iteration': None,
            #'Discharge Time [s]': None,
            #'Charge Time [s]': None,
            'Total Time [s]': None,
            #'High Voltage (Charge) [mV]': None,
            #'Low Voltage (Charge) [mV]': None,
            #'High Voltage (Discharge) [mV]': None,
            #'Low Voltage (Discharge) [mV]': None,
            'Result': None
        }
filename = "pippo.csv"

csvfile = open(filename,'w')

csvwriter = csv.writer(csvfile)

csvwriter.writerow(csv_log_dict.keys())
csvwriter.writerow(csv_log_dict.values())

csvfile.cl
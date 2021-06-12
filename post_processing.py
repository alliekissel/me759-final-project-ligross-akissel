import os
import numpy as np
from numpy.core.numeric import full
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sb

job_id = 14041084   # CHANGE THIS depending on ID
num_trials = 10   # CHANGE THIS depending on number of trials for job
thread_study = True     # change to false if this data isn't included
#plot_singles = True
plot_average = True

N_vals = np.array([10**4, 10**5, 10**6, 10**7, 10**8, 10**9, 10**10])

dir_path = os.path.dirname(os.path.realpath(__file__))
input_path = dir_path + '/first_timing/'          # CHANGE THIS depending on folder
output_path = dir_path + '/output/first_timing/'

full_serial_timing = []
full_parallel_timing = []

def process_results(args):
    id, index = args        # id is the job id, index is the job trial number (0-num_trials)

    for f in os.listdir(input_path):
        if f == 'job_{}_{}.out'.format(id, index):
            if thread_study:
                # serial data
                df_serial = pd.read_csv(input_path + f, header=None, skiprows=1, nrows=len(N_vals))
                df_serial.columns = ['Flux', 'RE', 'Histories Time (ms)', 'Estimator Time (ms)', 'Total Time (ms)']
                print(df_serial)
                # parallel data      
                df_parallel = pd.read_csv(input_path + f, header=None, skiprows=31)
                df_parallel.columns = ['Flux', 'RE', 'Histories Time (ms)', 'Estimator Time (ms)', 'Total Time (ms)']
                print(df_parallel)

            else:
                # serial data
                df_serial = pd.read_csv(input_path + f, header=None, skiprows=1, nrows=len(N_vals))
                df_serial.columns = ['Flux', 'RE', 'Histories Time (ms)', 'Estimator Time (ms)', 'Total Time (ms)']
                print(df_serial)
                # parallel data    
                df_parallel = pd.read_csv(input_path + f, header=None, skiprows=10)
                df_parallel.columns = ['Flux', 'RE', 'Histories Time (ms)', 'Estimator Time (ms)', 'Total Time (ms)']
                print(df_parallel)
            
            serial_timing = df_serial[["Total Time (ms)"]].to_numpy().reshape(1, len(N_vals))
            full_serial_timing.append(serial_timing.flatten())
            parallel_timing = df_parallel[["Total Time (ms)"]].to_numpy().reshape(1, len(N_vals))
            full_parallel_timing.append(parallel_timing.flatten())

    print('Results from job_{}_{} processed'.format(id, index))


# Create tasks to process
jobs = []

for job in [job_id]:        # make job_id a list if you want to process more than one job at once 
    for i in range(num_trials):    
        jobs.append((job, i))

for job in jobs:
    process_results(job)

# Create data frame containing all job trial data 
df_full_serial = pd.DataFrame(full_serial_timing, columns=N_vals)
df_full_parallel = pd.DataFrame(full_parallel_timing, columns=N_vals)
print("Serial data from all job trials, row=job_trial, column=N:")
print(df_full_serial)
print("Parallel data from all job trials, row=job_trial, column=N:")
print(df_full_parallel)

if plot_average == True:
    df_full_serial_average = df_full_serial.mean(axis=0)
    df_full_serial_stddev = df_full_serial.std(axis=0)
    df_full_parallel_average = df_full_parallel.mean(axis=0)
    df_full_parallel_stddev = df_full_parallel.std(axis=0)
    print("Average serial data over all job trials, row=N:")
    print(df_full_serial_average)
    print("Average parallel data over all job trials, row=N:")
    print(df_full_parallel_average) 

    # plot average
    df_full_average_data = pd.concat([df_full_serial_average, df_full_parallel_average], axis=1)
    df_full_stddev_data = pd.concat([df_full_serial_stddev, df_full_parallel_stddev], axis=1)
    df_full_average_data.columns=['Serial', 'Parallel']
    print("Average data over all job trials, row=N:")
    print(df_full_average_data)
    _, ax = plt.subplots()
    average_plot = sb.lineplot(data=df_full_average_data, palette=['blue', 'orange'])
    average_plot.set(yscale='log')
    average_plot.set(xscale='log')
    average_plot.set_title("Timing results averaged over {} independent jobs".format(num_trials))
    average_plot.set_xlabel("N")
    average_plot.set_ylabel("time (ms)")
    plt.errorbar(N_vals,
                 df_full_serial_average.to_numpy().reshape(1, len(N_vals)).flatten(),
                 yerr=2*df_full_serial_stddev, elinewidth=1,
                 capsize=5, ecolor="blue", ls='none')
    plt.errorbar(N_vals,
                 df_full_parallel_average.to_numpy().reshape(1, len(N_vals)).flatten(),
                 yerr=2*df_full_parallel_stddev, elinewidth=1,
                 capsize=5, ecolor="orange", ls='none')
    plt.show()

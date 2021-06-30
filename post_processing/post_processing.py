import os
import numpy as np
from numpy.core.numeric import full
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sb

job_id = 14041084   # CHANGE THIS depending on ID
num_trials = 10   # CHANGE THIS depending on number of trials for job
thread_study = True     # change to false if this data isn't included
plot_average = False
plot_flux = False
plot_RE = False

N_vals = np.array([10**4, 10**5, 10**6, 10**7, 10**8, 10**9, 10**10])

dir_path = os.path.dirname(os.path.realpath(__file__))
input_path = './results/first_timing/'          # CHANGE THIS depending on folder

full_serial_timing = []
full_parallel_timing = []
full_serial_flux = []
full_parallel_flux = []
full_serial_RE = []
full_parallel_RE = []

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

            serial_flux = df_serial[["Flux"]].to_numpy().reshape(1, len(N_vals))
            full_serial_flux.append(serial_flux.flatten())
            parallel_flux = df_parallel[["Flux"]].to_numpy().reshape(1, len(N_vals))
            full_parallel_flux.append(parallel_flux.flatten())

            serial_RE = df_serial[["RE"]].to_numpy().reshape(1, len(N_vals))
            full_serial_RE.append(serial_RE.flatten())
            parallel_RE = df_parallel[["RE"]].to_numpy().reshape(1, len(N_vals))
            full_parallel_RE.append(parallel_RE.flatten())

    print('Results from job_{}_{} processed'.format(id, index))


# Create tasks to process
jobs = []

for job in [job_id]:        # make job_id a list if you want to process more than one job at once 
    for i in range(num_trials):    
        jobs.append((job, i))

for job in jobs:
    process_results(job)


# Create data frame containing all job trial timing data 
df_full_serial_timing = pd.DataFrame(full_serial_timing, columns=N_vals)
df_full_parallel_timing = pd.DataFrame(full_parallel_timing, columns=N_vals)
print("Serial timing data from all job trials, row=job_trial, column=N:")
print(df_full_serial_timing)
print("Parallel timing data from all job trials, row=job_trial, column=N:")
print(df_full_parallel_timing)

# Speedups for each individual trial
df_full_speedup = df_full_serial_timing / df_full_parallel_timing
print("Speedup from all job trials, row=job_trial, column=N:")
print(df_full_speedup)

# Speedups for each individual trial
df_full_speedup_average = df_full_serial_timing.mean(axis=0) / df_full_parallel_timing.mean(axis=0)
print("Speedup from all job trials, row=job_trial, column=N:")
print(df_full_speedup_average)

if plot_average == True:
    df_full_serial_average = df_full_serial_timing.mean(axis=0)
    df_full_serial_stddev = df_full_serial_timing.std(axis=0)
    df_full_parallel_average = df_full_parallel_timing.mean(axis=0)
    df_full_parallel_stddev = df_full_parallel_timing.std(axis=0)
    print("Average timing serial data over all job trials, row=N:")
    print(df_full_serial_average)
    print("Average timing parallel data over all job trials, row=N:")
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
                 capsize=5, ecolor="blue", ls='none', lw=1)
    plt.errorbar(N_vals,
                 df_full_parallel_average.to_numpy().reshape(1, len(N_vals)).flatten(),
                 yerr=2*df_full_parallel_stddev, elinewidth=1,
                 capsize=5, ecolor="orange", ls='none', lw=1)
    plt.show()


# Create data frame containing all job trial flux data 
df_full_serial_flux = pd.DataFrame(full_serial_flux, columns=N_vals)
df_full_parallel_flux = pd.DataFrame(full_parallel_flux, columns=N_vals)
print("Serial flux data from all job trials, row=job_trial, column=N:")
print(df_full_serial_flux)
print("Parallel flux data from all job trials, row=job_trial, column=N:")
print(df_full_parallel_flux)

# Create data frame containing all job trial RE data 
df_full_serial_RE = pd.DataFrame(full_serial_RE, columns=N_vals)
df_full_parallel_RE = pd.DataFrame(full_parallel_RE, columns=N_vals)
print("Serial RE data from all job trials, row=job_trial, column=N:")
print(df_full_serial_RE)
print("Parallel RE data from all job trials, row=job_trial, column=N:")
print(df_full_parallel_RE)

# plot how flux (average flux between runs) changes as N increases
if plot_flux == True:
    df_full_serial_average = df_full_serial_flux.mean(axis=0)
    #df_full_serial_error = df_full_serial_RE.mean(axis=0)
    df_full_parallel_average = df_full_parallel_flux.mean(axis=0)
    #df_full_parallel_error = df_full_parallel_RE.mean(axis=0)
    print("Average flux serial data over all job trials, row=N:")
    print(df_full_serial_average)
    print("Average flux parallel data over all job trials, row=N:")
    print(df_full_parallel_average) 

    # plot average
    df_full_average_data = pd.concat([df_full_serial_average, df_full_parallel_average], axis=1)
    #df_full_error_data = pd.concat([df_full_serial_RE, df_full_parallel_RE], axis=1)
    df_full_average_data.columns=['Serial', 'Parallel']
    print("Average data over all job trials, row=N:")
    print(df_full_average_data)
    _, ax = plt.subplots()
    average_plot = sb.lineplot(data=df_full_average_data, palette=['blue', 'orange'])
    #average_plot.set(yscale='log')
    average_plot.set(xscale='log')
    average_plot.set_title("Flux results averaged over {} independent jobs".format(num_trials))
    average_plot.set_xlabel("N")
    average_plot.set_ylabel("Flux")
    # plt.errorbar(N_vals,
    #              df_full_serial_average.to_numpy().reshape(1, len(N_vals)).flatten(),
    #              yerr=df_full_serial_error/10000, elinewidth=1,
    #              capsize=5, ecolor="blue", ls='none')
    # plt.errorbar(N_vals,
    #              df_full_parallel_average.to_numpy().reshape(1, len(N_vals)).flatten(),
    #              yerr=df_full_parallel_error/10000, elinewidth=1,
    #              capsize=5, ecolor="orange", ls='none')
    plt.show()

# plot how RE (average RE between runs) changes as N increases
if plot_RE == True:
    df_full_serial_average = df_full_serial_RE.mean(axis=0)
    df_full_parallel_average = df_full_parallel_RE.mean(axis=0)
    print("Average RE serial data over all job trials, row=N:")
    print(df_full_serial_average)
    print("Average RE parallel data over all job trials, row=N:")
    print(df_full_parallel_average) 

    # plot average
    df_full_average_data = pd.concat([df_full_serial_average, df_full_parallel_average], axis=1)
    df_full_average_data.columns=['Serial', 'Parallel']
    print("Average data over all job trials, row=N:")
    print(df_full_average_data)
    _, ax = plt.subplots()
    average_plot = sb.lineplot(data=df_full_average_data, palette=['blue', 'orange'])
    #average_plot.set(yscale='log')
    average_plot.set(xscale='log')
    average_plot.set_title("RE results averaged over {} independent jobs".format(num_trials))
    average_plot.set_xlabel("N")
    average_plot.set_ylabel("RE")
    plt.show()
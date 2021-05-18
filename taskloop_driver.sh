#!/usr/bin/env bash
#SBATCH --job-name=taskloopsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="taskloop_sim_timing.out"
#SBATCH --error="taskloop_sim_timing.err"



g++ mc_driver_taskloop.cpp rng.cpp -o simulation_taskloop -fopenmp -g 
# thread analysis
for t in {1..10}
do
    ./simulation_taskloop $((10**4)) $((t))
done

# scailing analysis for best performing number of threads
# for i in {4..10}
# do
#     ./simulation_taskloop $((10**i)) THREAD_NO
# done
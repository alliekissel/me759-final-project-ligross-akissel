#!/usr/bin/env bash
#SBATCH --job-name=taskpsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="taskp_sim_timing.out"
#SBATCH --error="taskp_sim_timing.err"


g++ -fopenmp mc_driver_taskp.cpp rng.cpp -o simulation_taskp -g 
# thread analysis
for t in {1..10}
do
    ./simulation_taskp $((10**4)) $((t))
done

# scailing analysis for best performing number of threads
# for i in {4..10}
# do
#     ./simulation_taskp $((10**i)) THREAD_NO
# done
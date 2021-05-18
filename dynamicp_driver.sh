#!/usr/bin/env bash
#SBATCH --job-name=dynamicpsim
#SBATCH -p wacc
#SBATCH --time=0-00:10:00
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="dynamicp_sim_timing.out"
#SBATCH --error="dynamicp_sim_timing.err"


g++ mc_driver_dynamicp.cpp rng.cpp -o simulation_dynamicp -fopenmp
# thread analysis
for t in {1..10}
do
    ./simulation_dynamicp $((10**4)) $((t))
done

# scailing analysis for best performing number of threads
# for i in {4..10}
# do
#     ./simulation_dynamicp $((10**i)) THREAD_NO
# done
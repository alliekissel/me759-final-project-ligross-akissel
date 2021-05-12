#!/usr/bin/env bash
#SBATCH --job-name=taskloopsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="taskloop_sim_timing.out"
#SBATCH --error="taskloop_sim_timing.err"


# g++ mc_driver_taskloop.cpp rng.cpp -o simulation_taskloop
# ./simulation_taskloop 10000 5
# TODO add scaling loop
# for debugging
g++ -fopenmp mc_driver_taskloop.cpp rng.cpp -o simulation_taskloop -g 
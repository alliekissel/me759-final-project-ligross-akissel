#!/usr/bin/env bash
#SBATCH --job-name=taskpsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="taskp_sim_timing.out"
#SBATCH --error="taskp_sim_timing.err"


# g++ mc_driver_taskp.cpp rng.cpp -o simulation_taskp
# ./simulation_taskp 10000 5
# TODO add scaling loop
# for debugging
g++ -fopenmp mc_driver_taskp.cpp rng.cpp -o simulation_taskp -g 
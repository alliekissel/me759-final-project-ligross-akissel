#!/usr/bin/env bash
#SBATCH --job-name=dynamicpsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="dynamicp_sim_timing.out"
#SBATCH --error="dynamicp_sim_timing.err"


# g++ mc_driver_dynamicp.cpp rng.cpp -o simulation_dynamicp
# ./simulation_dynamicp 10000 5
# TODO add scaling loop
# for debugging
g++ -fopenmp mc_driver_dynamicp.cpp rng.cpp -o simulation_dynamicp -g 
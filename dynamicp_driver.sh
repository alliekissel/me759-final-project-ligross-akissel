#!/usr/bin/env bash
#SBATCH --job-name=dynamicpsim
#SBATCH -p wacc
#SBATCH --time=0-00:10:00
#SBATCH --nodes=1 --cpus-per-task=10
#SBATCH --output="dynamicp_sim_timing.out"
#SBATCH --error="dynamicp_sim_timing.err"


g++ mc_driver_dynamicp.cpp rng.cpp -o simulation_dynamicp -fopenmp
./simulation_dynamicp 10000 10
# TODO add scaling loop
# for debugging
# g++ mc_driver_dynamicp.cpp rng.cpp -o simulation_dynamicp -fopenmp -g 
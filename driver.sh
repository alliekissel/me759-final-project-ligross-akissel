#!/usr/bin/env bash
#SBATCH --job-name=serialsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="serial_sim.out"
#SBATCH --error="serial_sim.err"


g++ mc_driver.cpp rng.cpp -o simulation
./simulation 100 1

# for debugging
# g++ mc_driver.cpp rng.cpp -o simulation -g 
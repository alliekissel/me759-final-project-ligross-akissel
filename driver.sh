#!/usr/bin/env bash
#SBATCH --job-name=serialsim
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="serial_sim_test_scores.out"
#SBATCH --error="serial_sim_test_scores.err"


g++ mc_driver_serial.cpp rng.cpp -o test_simulation
./test_simulation 20 1
# TODO add scaling loop
# for debugging
# g++ mc_driver.cpp rng.cpp -o simulation -g 
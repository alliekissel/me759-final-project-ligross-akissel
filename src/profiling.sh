#!/usr/bin/env bash
#SBATCH --job-name=serialsim_profile
#SBATCH -p wacc
#SBATCH --time=0-00:10:00
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="profile_sim.out"
#SBATCH --error="profile_sim.err"


g++ -pg mc_driver.cpp rng.cpp -o simulation
./simulation 100 1

gprof simulation gmon.out
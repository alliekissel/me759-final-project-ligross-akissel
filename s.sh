#!/usr/bin/env bash
#SBATCH --job-name=checker
#SBATCH -p wacc
#SBATCH --time=0-00:02:30
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="sphere_checker.out"
#SBATCH --error="sphere_checker.err"


g++ sphere_checker.cpp -o sphere_checker
./sphere_checker
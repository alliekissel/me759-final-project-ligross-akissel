#!/usr/bin/env bash
#SBATCH --job-name=serialsim
#SBATCH -p wacc
#SBATCH --time=0-00:10:00
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="serial_sim_2.out"
#SBATCH --error="serial_sim.err"


g++ mc_driver_serial.cpp rng.cpp -o test_simulation
for i in {9..10}
do
    ./test_simulation $((10**i)) 1
done


# for debugging
# g++ mc_driver.cpp rng.cpp -o simulation -g 
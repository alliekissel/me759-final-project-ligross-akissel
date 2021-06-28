#!/usr/bin/env bash
#SBATCH --job-name=serialsim
#SBATCH -p wacc
#SBATCH --time=0-00:30:00
#SBATCH --nodes=1 --cpus-per-task=1
#SBATCH --output="serial_sim_2.out"
#SBATCH --error="serial_sim.err"

# Commands to enable modules, and then load an appropriate MP/MPI module
export PATH
. /etc/profile.d/modules.sh
module load GCC/8.3.0 # OpenMP contained in GCC
g++ mc_driver_serial.cpp rng.cpp -std=c++11 -o serial_simulation 
echo "serial"
for i in {4..10}
do
    ./serial_simulation $((10**i)) 1
done
echo "dynamic p"
# echo "thread study"
# g++ mc_driver_dynamicp.cpp rng.cpp -std=c++11 -fopenmp -o dynamic_p_simulation
# for t in {1..20}
# do
#     ./dynamic_p_simulation $((10**6)) $((t))
# done
echo "dynamic timing"
t_best=12
for i in {4..10}
do
    ./dynamic_p_simulation $((10**i)) $((t_best))
done
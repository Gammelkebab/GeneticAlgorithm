#!/bin/bash -login
#PBS -N MPI
#PBS -j oe
#PBS -m ae
#PBS -l nodes=4:ppn=2
#PBS -l walltime=00:01:00
#PBS -l mem=4gb

module load intel

cd GeneticAlgorithm

make

./main

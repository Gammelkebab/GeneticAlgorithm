#!/bin/bash -login
#PBS -N GeneticAlgorithm
#PBS -j oe
#PBS -m ae
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:20:00
#PBS -l mem=4gb

module load intel

cd GeneticAlgorithm

make

./main

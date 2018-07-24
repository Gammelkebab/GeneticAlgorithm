#!/bin/bash -login
#PBS -N Benchmark_GA
#PBS -j oe
#PBS -m ae
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:20:00
#PBS -l mem=4gb

module load intel

cd GeneticAlgorithm/Genetic_Algorithm_Benchmark

make

./main

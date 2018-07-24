#!/bin/bash -login
#PBS -N GeneticAlgorithm_Benchmark
#PBS -j oe
#PBS -m ae
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:20:00
#PBS -l mem=4gb
#PBS -o benchmark_log.txt

module load intel

cd GeneticAlgorithm

make

./main

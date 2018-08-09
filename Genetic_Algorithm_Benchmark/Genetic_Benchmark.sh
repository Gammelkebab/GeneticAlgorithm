#!/bin/bash -login
#PBS -N Benchmark_GA
#PBS -j oe
#PBS -m ae
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:20:00
#PBS -l mem=512mb
#PBS -W x=PARTITION:lena

RND=$RANDOM

cp -r GeneticAlgorithm/Genetic_Algorithm_Benchmark $BIGWORK/GeneticAlgorithm_Benchmark_$RND
cd $BIGWORK/GeneticAlgorithm_$RND
make

./main
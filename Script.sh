#!/bin/bash -login
#PBS -N GeneticAlgorithm
#PBS -j oe
#PBS -m ae
#PBS -l nodes=4:ppn=4
#PBS -l walltime=00:05:00
#PBS -l mem=1gb
#PBS -W x=PARTITION:lena

module load intel

cd GeneticAlgorithm

make

./main

#!/bin/bash -login
#PBS -N GeneticAlgorithm
#PBS -j oe
#PBS -m ae
#PBS -l nodes=4:ppn=4
#PBS -l walltime=00:20:00
#PBS -l mem=4gb
#PBS -W x=PARTITION:lena

module load intel

cd GeneticAlgorithm

make

./main

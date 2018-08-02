#!/bin/bash -login
#PBS -N GeneticAlgorithm
#PBS -j oe
#PBS -m ae
#PBS -l nodes=4:ppn=16
#PBS -l walltime=00:05:00
#PBS -l mem=512mb
#PBS -W x=PARTITION:lena

module load intel

cd $BIGWORK/GeneticAlgorithm

make

./main

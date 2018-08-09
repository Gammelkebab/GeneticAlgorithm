#!/bin/bash -login
#PBS -N GeneticAlgorithm
#PBS -j oe
#PBS -m ae
#PBS -l nodes=4:ppn=16
#PBS -l walltime=00:05:00
#PBS -l mem=512mb
#PBS -W x=PARTITION:lena

RND=$RANDOM

cp -r GeneticAlgorithm $BIGWORK/GeneticAlgorithm_$RND
cd $BIGWORK/GeneticAlgorithm

module load intel
make clean
make

./main
#!/bin/bash

#---------- Job name
#PBS -N test_jaeschke

#--------- Mail adress
#PBS -M natalie.jaeschke@tu-dortmund.de

#--------- mail for abort, begin, end :
#PBS -m abe

#--------- join STDOUT and STDERR
##PBS -j oe

#--------- redirect STDOUT and STDERR
#PBS -o ${HOME}/phd/openmp-tut/oe/stdout.${PBS_JOBID}
#PBS -e ${HOME}/phd/openmp-tut/oe/stderr.${PBS_JOBID}

#--------- estimated (maximum) runtime
#--------- [[hours:]minutes:]seconds[.milliseconds]
#PBS -l walltime=00:00:10

#---------- 1 core on one node
#PBS -l nodes=2:ppn=2
#---------- Memory estimate
#---------- (64-256 GB RAM per node)
#PBS -l vmem=1g

# -- OpenMP environment variables --
#OMP_NUM_THREADS=$PBS_NUM_PPN
OMP_NUM_THREADS=2
export OMP_NUM_THREADS

module load openmpi/gcc-4.7.1/1.6 

echo "PBS_JOBNAME=${PBS_JOBNAME}"
echo "PBS_JOBID=${PBS_JOBID}"
if [[ -r ${PBS_NODEFILE} ]]; then echo; cat ${PBS_NODEFILE}; fi
echo
echo "--- START --- $(date) --- "

echo "STDOUT $*"
echo "STDERR $*" >&2


cd ${HOME}/phd/openmp-tut
mpirun -n 2 -npernode 1 a.out
sleep 2


echo "--- END ----- $(date) ---"
echo
echo "$(whoami) is leaving from $(hostname) ..."
echo

#!/bin/bash -l

#-----The '#' followed by SBATCH are SLURM prefixes. They are not comments-----#
#-----It is worth taking note of them when you write your own batch files------#
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -p course
#SBATCH -t 5


#ADD INSTRUCTIONS TO LOAD THE MODULES HERE
module load compilers/intel/2019u5
module load mpi/intel-mpi/2019u5/bin

# SRC = $1 is name of the source code as an arguement
SRC=$1

# sets the exe name as the sourcecode, and %% removes the ".c"
EXE=${SRC%%.c}.exe

# delete the existing executable, if it exists
rm -f ${EXE}

echo compiling $SRC to $EXE

echo -------------------------


#ADD COMPILER INSTRUCTION HERE.
mpiicc -no-multibyte-chars $SRC -o $EXE

#SLURM_NTASKS is given by the -n flag when using sbatch.
procs=${SLURM_NTASKS:-1}


#RUN THE PROGRAM HERE USING $procs WITH THE -np FLAG.
mpirun -np $procs ./$EXE

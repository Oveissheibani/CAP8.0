#!/bin/bash
#conda init bash
#conda activate CAP_CONDA
#echo Using CONDA environment $CAP_CONDA
echo "========================================================================================"
echo The root version is:
echo `which root` 
TASKIX=$SLURM_ARRAY_TASK_ID
SEED=$(( SLURM_ARRAY_TASK_ID + SLURM_ARRAY_JOB_ID*100 ))
CAP_HISTOS_EXPORT_PATH=$CAP_WORKINGDIRECTORY_Output/$(printf "SUBBUNCH%03d/" $TASKIX )
echo "========================================================================================"
echo SLURM_ARRAY_TASK_ID is  $SLURM_ARRAY_TASK_ID
echo SLURM_ARRAY_JOB_ID  is  $SLURM_ARRAY_JOB_ID
echo TASKIX is  $TASKIX
echo SEED is $SEED
echo NEVENTS is $CAP_NEVENTS
echo Will save histograms at $CAP_HISTOS_EXPORT_PATH
mkdir $CAP_HISTOS_EXPORT_PATH
echo "Calling root w/ RunAna"
echo "========================================================================================"
root -b "$CAP_MACROS_PATH/RunAna.C("\"$CAP_JOB_CONFIGURATION\"","\"$CAP_HISTOS_EXPORT_PATH\"",$SEED,true,$CAP_NEVENTS,1,1)"


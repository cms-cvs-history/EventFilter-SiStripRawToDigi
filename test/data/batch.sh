#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch batch.sh $FILE1 
### Output root file copied to castor.

#arguments
FILE1_=$1;

#hardcoded directories
CFGDIR=~pwing/scratch0/CMSSW_1_3_1_HLT2/src/EventFilter/SiStripRawToDigi/data;
CASTORFILE=/castor/cern.ch/user/p/pwing/LowLevelRecon

#record current directory
WNDIR=$(pwd);

#set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#run
cmsRun $CFGDIR'/'$FILE1_;

#store output files
rfcp *.root $CASTORFILE;

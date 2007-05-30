#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch rawtoclusters.sh $1 $2 $3
### $1 = cfg file.
### $2 = CMSSW_BASE.
### $3 = output root file destination.

#record current directory
WNDIR=$(pwd);

#config directories
CFGDIR=$2/src/EventFilter/SiStripRawToDigi/data;

#set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#run
cmsRun $CFGDIR'/'$1;

#store output files
rfcp *.root $3;

#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch batch.sh $1 $2 $3 $4 $5 $6
### $1 = CMSSW_BASE.
### $2 = output root file destination.
### $3 = cfg file
### $4 = root file 1.
### $5 = root file 2.
### $6 = root file 3.

# record current directory
WNDIR=$(pwd);

# set environment
cd $1/src;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

# run
cmsRun $3;

# store output files
if [ "$4" != "" ];then
    rfcp $4 $2;
fi
if [ "$5" != "" ];then
    rfcp $5 $2;
fi
if [ "$6" != "" ];then
    rfcp $6 $2;
fi


#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch rawtoclusters.sh $1 $2 $3 $4 $5
### $1 = CMSSW_BASE.
### $2 = output root file destination.
### $3 = cfg file 1.
### $4 = cfg file 2.
### $5 = cfg file 3.

# record current directory
WNDIR=$(pwd);

# config directories
CFGDIR=$1/src/EventFilter/SiStripRawToDigi/test;

# set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

# copy over necessary files
cp /afs/cern.ch/cms/cmt/onlinedev/data/cabling/131/authentication.xml .

# run
cmsRun $CFGDIR'/'$3;
if [ "$4" != "" ];then
    cmsRun $CFGDIR'/'$4;
fi
if [ "$5" != "" ];then
    cmsRun $CFGDIR'/'$5;
fi

# store output files
rfcp *.root $2;

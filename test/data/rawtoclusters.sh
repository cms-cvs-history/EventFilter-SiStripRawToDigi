#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch rawtoclusters.sh $1 $2 $3 $4
### $1 = cfg file 1.
### $2 = cfg file 2.
### $3 = CMSSW_BASE.
### $4 = output root file destination.

#record current directory
WNDIR=$(pwd);

#config directories
CFGDIR=$3/src/EventFilter/SiStripRawToDigi/data;

#set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
touch $CFGDIR/output.txt
cp $CFGDIR/output.txt .
cp /afs/cern.ch/cms/cmt/onlinedev/data/cabling/131/authentication.xml .

#run
cmsRun $CFGDIR'/'$1;
if [ "$2" != "" ];then
    cmsRun $CFGDIR'/'$2;
fi

#store output files
rfcp *.root $4;
cp output.txt $CFGDIR;

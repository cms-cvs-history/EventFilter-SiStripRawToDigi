#!/bin/bash

#arguments
FILE1_=$1;
FILE2_=$2;

#hardcoded directories
CFGDIR=~pwing/scratch0/CMSSW_1_3_0_pre5/src/EventFilter/SiStripRawToDigi/test;
CASTORFILE=/castor/cern.ch/user/p/pwing/LowLevelRecon/Performance2.root

#record current directory
WNDIR=$(pwd);

#set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
cp $CFGDIR'/authentication.xml' .
cp $CFGDIR'/sistripfedcabling.db' .
cp $CFGDIR'/SiStripFedCablingCatalog.xml' .
cp $CFGDIR'/noise_0.0.db' .
cp $CFGDIR'/noise_0.0.xml' .

#run
cmsRun $CFGDIR'/'$FILE1_;
if [ "$FILE2" != "" ];then
    cmsRun $CFGDIR'/'$FILE2_;
fi

rfcp Performance.root $CASTORFILE;

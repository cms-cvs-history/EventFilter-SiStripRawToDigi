#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch rawtoclusters.sh $FILE1 $FILE2
### Output root file copied to castor, txt file to local config dir.

#arguments
FILE1_=$1;
FILE2_=$2;

#hardcoded directories
CFGDIR=~pwing/scratch0/CMSSW_1_5_0_pre4/src/EventFilter/SiStripRawToDigi/data;
CASTORFILE=/castor/cern.ch/user/p/pwing/LowLevelRecon
AUTHDIR=~pwing/public/scripts/RawToDigi

#record current directory
WNDIR=$(pwd);

#set environment
cd $CFGDIR;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
touch $CFGDIR/output.txt
cp $CFGDIR/output.txt .
cp $AUTHDIR/authentication.xml .

#run
cmsRun $CFGDIR'/'$FILE1_;
if [ "$FILE2" != "" ];then
    cmsRun $CFGDIR'/'$FILE2_;
fi

#store output files
rfcp *.root $CASTORFILE;
cp output.txt $CFGDIR

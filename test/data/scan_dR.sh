#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch scan_dR.sh $1 $2
### $1 = CMSSW_BASE. 
### $2 = output root file destination.

#record current directory
WNDIR=$(pwd);

#config directories
DATA=$1/src/EventFilter/SiStripRawToDigi/data;
TESTDATA=$1/src/EventFilter/SiStripRawToDigi/test/data;

#set environment
cd $DATA;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
cp /afs/cern.ch/cms/cmt/onlinedev/data/cabling/131/authentication.xml .

#run
namefield='TreeName'
valuefield='DeltaR'
equals=' = '
string='"'
endline=' #'

for dR in 0.1 0.2 0.3 0.4 0.5 
do

name=$string$dR$string;
value=$dR;

replace "$namefield" "$namefield$equals$name$endline" -- $TESTDATA/SiStripPerformanceRecordModule.cfi;
replace "$valuefield" "$valuefield$equals$value$endline" -- $TESTDATA/SiStripRawToClustersDummyRoI.cfi;
cmsRun $DATA/RegionalReconstruction.cfg;

done

#save root files
rfcp *.root $2

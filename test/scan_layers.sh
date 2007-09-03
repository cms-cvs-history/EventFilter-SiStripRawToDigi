#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch scan_layers.sh $1 $2 $3
### $1 = CMSSW_BASE. 
### $2 = output root file destination.
### $3 = output root file.

#record current directory
WNDIR=$(pwd);

#set environment
cd $1;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#config directories
DATA=$1/src/EventFilter/SiStripRawToDigi/data;
TEST=$1/src/HLTrigger/Configuration/test;
TESTDATA=$1/src/EventFilter/SiStripRawToDigi/test/data;

#useful values
namefield='TreeName'
layersfield='Layers'
equals=' = '
string='"'
endline=' #'

#run
for layers in 1 2 3 4 5 6 7 8 9 10 11 12 -1
do
name=$string$layers$string;
value=$layers;
replace "$namefield" "$namefield$equals$name$endline" -- $TESTDATA/SiStripPerformance.cfi;
replace "$layersfield" "$layersfield$equals$value$endline" -- $DATA/SiStripRawToClustersRoI.cfi;
cmsRun $TEST/HLTtable.cfg;
done

#save root files
rfcp $3 $2;

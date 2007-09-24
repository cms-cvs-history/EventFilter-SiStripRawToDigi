#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm scan_detadphi.sh $1 $2 $3
### $1 = CMSSW_BASE. 
### $2 = output root file destination.
### $3 = root file.

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
etafield='EtaWindow'
phifield='PhiWindow'
globalfield='Global'
equals=' = '
string='"'
endline=' #'

#run
for delta in 0.0 0.02 0.04 0.06 0.08 0.085 0.09 0.095 0.1 0.105 0.11 0.115 0.12 0.125 0.13 0.135 0.14 0.145 0.15 0.155 0.16 0.18 0.2 Global
do
name=$string$delta$string;
value=$delta;
replace "$namefield" "$namefield$equals$name$endline" -- $TESTDATA/SiStripPerformance.cfi;
replace "$etafield" "$etafield$equals$value$endline" -- $DATA/SiStripRoIDimensions.cfi;
replace "$phifield" "$phifield$equals$value$endline" -- $DATA/SiStripRoIDimensions.cfi;
replace "$globalfield" "$globalfield$equals""false""$endline" -- $DATA/SiStripRawToClustersRoI.cfi;
if [ $delta == "Global" ];then
    replace "$globalfield" "$globalfield$equals""true""$endline" -- $DATA/SiStripRawToClustersRoI.cfi;
    replace "$etafield" "$etafield$equals""0.0""$endline" -- $DATA/SiStripRoIDimensions.cfi;
    replace "$phifield" "$phifield$equals""0.0""$endline" -- $DATA/SiStripRoIDimensions.cfi;
fi
cmsRun $TEST/HLTtable.cfg;
done

#save root files
rfcp $3 $2

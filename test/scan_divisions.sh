#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm  -m lxb7350.cern.ch scan_divisions.sh $1 $2 $3
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
CONN=$1/src/CalibTracker/SiStripConnectivity/data;
TEST=$1/src/EventFilter/SiStripRawToDigi/test/data;
TESTDATA=$1/src/EventFilter/SiStripRawToDigi/test/data;

#useful values
namefield='TreeName'
etafield='EtaDivisions'
phifield='PhiDivisions'
equals=' = '
string='"'
endline=' #'

#run
for divisions in 1 2 3 4 5 10 20 50 100 500 1000 
do
name=$string$divisions$string;
value=$delta;
replace "$namefield" "$namefield$equals$name$endline" -- $TESTDATA/SiStripPerformance.cfi;
replace "$etafield" "$etafield$equals$value$endline" -- $CONN/SiStripRegionConnectivity.cfi;
replace "$phifield" "$phifield$equals$value$endline" -- $CONN/SiStripRegionConnectivity.cfi;
cmsRun $TEST/TrivialRegionalDigi2Raw2RecHits.cfg;
done

#save root files
rfcp $3 $2

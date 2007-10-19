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

#config files
CONNECTIVITY=$1/src/CalibTracker/SiStripConnectivity/data/SiStripRegionConnectivity.cfi;
PERFORMANCE=$1/src/EventFilter/SiStripRawToDigi/test/data/SiStripPerformance.cfi;
RUN=$1/src/EventFilter/SiStripRawToDigi/test/RegionalDigi2Raw2RecHits.cfg;

#backup config files
cp $CONNECTIVITY tmp1.txt;
cp $PERFORMANCE tmp2.txt;
cp $RUN tmp3.txt;

#useful values
namefield='TreeName'
etafield='EtaDivisions'
phifield='PhiDivisions'
equals=' = '
string='"'
endline=' #'

#run
for divisions in 1 10 20 30 40 50 60 70 80 90 100
do
name=$string$divisions$string;
value=$divisions;
replace "$namefield" "$namefield$equals$name$endline" -- $PERFORMANCE;
replace "$etafield" "$etafield$equals$value$endline" -- $CONNECTIVITY;
replace "$phifield" "$phifield$equals$value$endline" -- $CONNECTIVITY;
echo "scan_divisions.sh: Run setting " $name
cmsRun $RUN;
done

#revert config files
mv tmp1.txt $CONNECTIVITY;
mv tmp2.txt $PERFORMANCE;
mv tmp3.txt $RUN;

#save root files
rfcp $3 $2

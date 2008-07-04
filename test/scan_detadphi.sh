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

#config files
ROI=$1/src/EventFilter/SiStripRawToDigi/data/SiStripRawToClustersRoI.cfi;
PERFORMANCE=$1/src/EventFilter/SiStripRawToDigi/test/data/SiStripPerformance.cfi;
RUN=$1/src/HLTrigger/Configuration/test/HLTtable.cfg;

#backup config files
cp $ROI tmp1.txt;
cp $PERFORMANCE tmp2.txt;
cp $RUN tmp3.txt;

#useful values
namefield='TreeName'
etafield='EtaWindow'
phifield='PhiWindow'
equals=' = '
string='"'
endline=' #'

#run
for delta in 
#TAU-JETS 0.0 0.02 0.04 0.06 0.08 0.09 0.1 0.11 0.12 0.13 0.14 0.15 0.16 0.18 0.2 0.22 0.24 0.26 0.28 0.3 0.32 0.34 0.36 0.38 0.4 0.42 0.44 0.46 0.48 0.5
#B-JETS 0.0 0.02 0.04 0.06 0.08 0.09 0.1 0.11 0.12 0.13 0.14 0.15 0.16 0.18 0.2 0.22 0.24 0.26 0.28 0.3 0.32 0.34 0.36 0.38 0.4 0.42 0.44 0.46 0.48 0.5
#LEPTONS 0.0 0.02 0.04 0.06 0.08 0.095 0.1 0.105 0.11 0.115 0.12 0.125 0.13 0.135 0.14 0.145 0.15 0.16 0.18 0.2 
do
name=$string$delta$string;
value=$delta;
replace "$etafield" "$etafield$equals$value$endline" -- $ROI;
replace "$phifield" "$phifield$equals$value$endline" -- $ROI;
replace "$namefield" "$namefield$equals$name$endline" -- $PERFORMANCE;
echo "scan_detadphi.sh: Run setting " $name
cmsRun $RUN;
done

#revert config files
mv tmp1.txt $ROI;
mv tmp2.txt $PERFORMANCE;
mv tmp3.txt $RUN;

#save root files
rfcp $3 $2
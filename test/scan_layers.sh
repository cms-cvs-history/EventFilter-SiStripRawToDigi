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
layersfield='Layers'
equals=' = '
string='"'
endline=' #'

#run
for layers in 1 2 3 4 5 6 7 8 9 10 11 12 -1
do
name=$string$layers$string;
value=$layers;
replace "$layersfield" "$layersfield$equals$value$endline" -- $ROI;
replace "$namefield" "$namefield$equals$name$endline" -- $PERFORMANCE;
cmsRun $RUN;
done

#revert config files
mv tmp1.txt $ROI;
mv tmp2.txt $PERFORMANCE;
mv tmp3.txt $RUN;

#save root files
rfcp $3 $2;

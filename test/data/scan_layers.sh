#!/bin/bash

### Run using the following syntax:
### bsub -q 8nm -m lxb7350.cern.ch scan_dR.sh $1 $2
### $1 = CMSSW_BASE. 
### $2 = output root file destination.

#record current directory
WNDIR=$(pwd);

#config directories
DATA=$1/src/EventFilter/SiStripRawToDigi/data;
CONN=$1/src/CalibTracker/SiStripConnectivity/data/SiStripRegionConnectivity.cfi;

#set environment
cd $DATA;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
cp /afs/cern.ch/cms/cmt/onlinedev/data/cabling/131/authentication.xml .

#useful variables
replacefull='replace SiStripRegionConnectivity.SiStripFull.'
tib=$replacefull'TIBLayers = {1,2,3,4};'
tob=$replacefull'TOBLayers = {1,2,3,4,5,6};'
tec=$replacefull'TECLayers = {1,2,3,4,5,6,7,8,9};'
tid=$replacefull'TIDLayers = {1,2,3};'

#run
cp $CONN conn.txt
for layers in $tob $tob$tec $tib$tid $tib$tid$tob $tib$tid$tob$tec
do

sed '$a'"$layers" < $CONN > tmp.cfg; mv tmp.txt $CONN;
cmsRun $DATA/RegionalReconstruction.cfg;
cp conn.txt $CONN

done

#save root files
rfcp *.root $2

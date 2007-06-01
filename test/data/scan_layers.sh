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
CONN=$1/src/CalibTracker/SiStripConnectivity/data/SiStripRegionConnectivity.cfi;

#set environment
cd $DATA;
source /afs/cern.ch/cms/sw/cmsset_default.sh;
eval `scramv1 runtime -sh`;
cd $WNDIR;

#copy over necessary files
cp /afs/cern.ch/cms/cmt/onlinedev/data/cabling/131/authentication.xml .

#useful values
replace='replace SiStripRegionConnectivity.'
empty=$replace'TIBLayers={};'$replace'TOBLayers={};'$replace'TECLayers={};'$replace'TIDLayers={};'
namefield='TreeName'
equals=' = '
string='"'
endline=' #'

#available settings
tib=$replace'TIBLayers={1,2,3,4};'
tob=$replace'TOBLayers={1,2,3,4,5,6};'
tec=$replace'TECLayers={1,2,3,4,5,6,7,8,9};'
tid=$replace'TIDLayers={1,2,3};'
tobouter=$replace'TOBLayers={4,5,6};'
tecouter=$replace'TECLayers={7,8,9};'

#run
cp $CONN conn.txt;
for layers in "$tob$tec" "$tib$tid" "$tib$tid$tob$tec" "$tobouter$tecouter" "$tib$tid$tobouter$tecouter" 
do

sed '$a'"$empty" < $CONN > tmp.txt;
sed '$a'"$layers" < tmp.txt > $CONN;

layers=${layers//$replace};
layers=${layers//"Layers={"/":["};
layers=${layers//"};"/"] "};
name=$string$layers$string
replace "$namefield" "$namefield$equals$name$endline" -- $TESTDATA/SiStripPerformanceRecordModule.cfi;

cmsRun $DATA/RegionalReconstruction.cfg;
cp conn.txt $CONN;

done

#save root files
rfcp *.root $2;

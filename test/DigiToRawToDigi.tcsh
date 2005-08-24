#!/bin/tcsh

echo "stdout will be piped to stdout.DigiToRawToDigi "
eval `scramv1 runtime -csh`
rehash
if ( -e "stdout.DigiToRawToDigi" ) then
 mv stdout.DigiToRawToDigi stdout.DigiToRawToDigi.bak
endif
cmsRun --parameter-set DigiToRawToDigi.txt > & stdout.DigiToRawToDigi &


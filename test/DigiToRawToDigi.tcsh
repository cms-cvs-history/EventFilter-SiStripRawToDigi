#!/bin/tcsh

echo "stdout will be piped to log.DigiToRawToDigi "
eval `scramv1 runtime -csh`
rehash
if ( -e "log.DigiToRawToDigi" ) then
 mv log.DigiToRawToDigi log.DigiToRawToDigi.bak
endif
cmsRun --parameter-set DigiToRawToDigi.txt > & log.DigiToRawToDigi &


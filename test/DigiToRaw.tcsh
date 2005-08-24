#!/bin/tcsh

echo "stdout will be piped to log.DigiToRaw "
eval `scramv1 runtime -csh`
rehash
if ( -e "log.DigiToRaw" ) then
 mv log.DigiToRaw log.DigiToRaw.bak
endif
cmsRun --parameter-set DigiToRaw.txt > & log.DigiToRaw &


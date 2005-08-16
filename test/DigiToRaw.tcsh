#!/bin/tcsh

echo "stdout will be piped to stdout.DigiToRaw "
eval `scramv1 runtime -csh`
rehash
if ( -e "stdout.DigiToRaw" ) then
 mv stdout.DigiToRaw stdout.DigiToRaw.bak
endif
cmsRun --parameter-set DigiToRaw.txt > & stdout.DigiToRaw &


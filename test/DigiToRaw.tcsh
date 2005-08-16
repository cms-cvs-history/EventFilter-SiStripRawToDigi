#!/bin/tcsh

echo "stdout will be piped to stdout.DigiToRaw "
eval `scramv1 runtime -csh`
rehash
cmsRun --parameter-set DigiToRaw.txt > & stdout.DigiToRaw &


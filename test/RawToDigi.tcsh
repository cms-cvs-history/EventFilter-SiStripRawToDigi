#!/bin/tcsh

echo "stdout will be piped to stdout.RawToDigi "
eval `scramv1 runtime -csh`
rehash
cmsRun --parameter-set RawToDigi.txt > & stdout.RawToDigi &


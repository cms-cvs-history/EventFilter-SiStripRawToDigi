#!/bin/tcsh

echo "stdout will be piped to stdout.RawToDigi "
eval `scramv1 runtime -csh`
rehash
if ( -e "stdout.RawToDigi" ) then
  mv stdout.RawToDigi stdout.RawToDigi.bak
endif
cmsRun --parameter-set RawToDigi.txt > & stdout.RawToDigi &


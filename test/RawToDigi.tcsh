#!/bin/tcsh

echo "stdout will be piped to log.RawToDigi "
eval `scramv1 runtime -csh`
rehash
if ( -e "log.RawToDigi" ) then
  mv log.RawToDigi log.RawToDigi.bak
endif
cmsRun --parameter-set RawToDigi.cfg > & log.RawToDigi &


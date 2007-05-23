#ifndef EventFilter_SiStripRawToDigi_PerformanceOverlay_H
#define EventFilter_SiStripRawToDigi_PerformanceOverlay_H

#include <string>
#include "TTree.h"
#include "TRFIOFile.h"
#include "TProfile.h"
#include <vector>
#include "EventFilter/SiStripRawToDigi/test/stubs/Overlay.h"

class PerformanceOverlay {

public:
  
  PerformanceOverlay(TRFIOFile*);
  ~PerformanceOverlay();

  void book();
  void unbook();
  void overlay(std::string);

private:

  TRFIOFile* file_;
  Overlay* overlay_;

};

#endif
